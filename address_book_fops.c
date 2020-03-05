#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>

#include "address_book.h"

Status load_file(AddressBook* address_book)
{
		if ((address_book -> fp = fopen(DEFAULT_FILE, "r")) == NULL)
		{
				perror("open");
				printf("Address book file does not exist!\n");
				return e_fail;
		}
		//Get the contacts' count from the file & store in AddressBook structure
		address_book -> count = 0;
		fseek(address_book -> fp, 0L, SEEK_END);
		if (ftell(address_book -> fp) == 0)
				//printf("First byte = Last byte = EOF\n");
				return e_success;
		while (ftell(address_book -> fp) != 0)
		{
				if (fgetc(address_book -> fp) == NEXT_ENTRY)
						++(address_book -> count);
				fseek(address_book -> fp, -2L, SEEK_CUR);
		}
		//printf("No. of contacts: %d\n", address_book -> count);
		return e_success;
}

Status save_file(AddressBook* address_book)
{
		if ((address_book -> fp = freopen(DEFAULT_FILE, "rw+", address_book -> fp)) == NULL)
		{
				perror("open");
				exit(1);
		}
		fseek(address_book -> fp, -1L, SEEK_END);
		/* 
		 * Add the logic to save the file
		 * Make sure to do error handling
		 */ 
		int name_len = strlen(address_book -> list -> name[NAME_COUNT - 1]);
		fwrite(address_book -> list -> name[NAME_COUNT - 1], name_len, 1, address_book -> fp);
		for (int i = 0; i < PHONE_NUMBER_COUNT; i++)
		{
				fprintf(address_book -> fp, "%c", FIELD_DELIMITER);
				fwrite(address_book -> list -> phone_numbers[i], 10, 1, address_book -> fp);
		}
		for (int i = 0; i < EMAIL_ID_COUNT; i++)
		{
				fprintf(address_book -> fp, "%c", FIELD_DELIMITER);
				int email_len = strlen(address_book -> list -> email_addresses[i]);
				fwrite(address_book -> list -> email_addresses[i], email_len, 1, address_book -> fp);
		}
		//Move to Next entry. Give it twice as first time it goes into buffer
		fprintf(address_book -> fp, "%c", NEXT_ENTRY);
		fprintf(address_book -> fp, "%c", NEXT_ENTRY);
		++address_book -> count;
		return e_success;
}

int search_serial_num(AddressBook* address_book, char* search_str, int sr_no)
{
		int line_count = 0;
		fseek(address_book -> fp, 0L, SEEK_END);
		long eof = ftell(address_book -> fp);
		//printf("Last EOF byte offset = %ld\n", eof);
		//Find string
		int len = strlen(search_str);
		char found = 0;
		//printf("len = %d\n", len);
		char* str = calloc(len + 1, sizeof(char));
		rewind(address_book -> fp);
		while (ftell(address_book -> fp) != eof - len)
		{
				fread(str, len, 1, address_book -> fp);
				if (str[0] == '\n')
						line_count++;
				if (sr_no == line_count + 1)
						break;
				if (!strcasecmp(str, search_str))
				{
						//printf("%s\n", str);
						found = 1;
						break;
				}
				fseek(address_book -> fp, (1 - len), SEEK_CUR);
		}
		//printf("File offset is at %ld\n", ftell(address_book -> fp));
		free(str);
		if (found) 
		{
				fseek(address_book -> fp, (-1 * len), SEEK_CUR);
				//printf("String found at file offset: %ld\n", ftell(address_book -> fp));
		}
		else if (sr_no == line_count + 1)
		{
				fseek(address_book -> fp, (1 - len), SEEK_CUR);
				//printf("Sr. num found!\nFile offset of beginning of contact: %ld\n", ftell(address_book -> fp));
		}
		else
		{
				//printf("String not found!\n");
				return -1;
		}
		/*
		   char ch = fgetc(address_book -> fp);
		   printf("%c\n", ch);
		   fseek(address_book -> fp, -1L, SEEK_CUR);
		 */
		return line_count + 1;
}

