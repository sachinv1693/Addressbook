#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#include "address_book_fops.h"
#include "address_book_menu.h"
#include "address_book.h"

char get_option(int type, const char* msg)
{
		/*
		 * Mutilfuction user intractions like
		 * Just an enter key detection
		 * Read an number
		 * Read a charcter
		 Fill the code to add above functionality */
		char ch;
		if (type == NONE)
		{
				printf("%s", msg);
				scanf("%c", &ch);
				return (ch == '\n') ? e_no_opt : ch;
		}
		else if (type == NUM)
		{
				scanf("%d", &type);//Let's use the same type variable
				return type;
		}
		else//type === CHAR
		{
				printf("%s", msg);
				scanf("%c", &ch);
				return (ch == '\n') ? e_no_opt : ch;
		}
}

Status save_prompt(AddressBook* address_book)
{
		if (strlen(address_book -> list -> name[NAME_COUNT - 1]) == 0)
		{
				return e_fail;
		}
		char option = get_option(CHAR, "\rEnter 'n or N' to Ignore and 'y or Y' to Save changes: ");
		if (option == 'y' || option == 'Y')
		{
				save_file(address_book);
				printf("Data successfully saved in file: %s\n", DEFAULT_FILE);
				return e_success;
		}
		else if (option == 'n' || option == 'N')
				return e_fail;
		else
				return save_prompt(address_book);
}

void menu_header(const char* str)
{
		fflush(stdout);
		system("clear");
		printf("#######  Address Book  #######\n");
		if (*str != '\0')
		{
				printf("#######  %s\n", str);
		}
}

void main_menu(void)
{
		menu_header("Features:\n");
		printf("0. Exit\n");
		printf("1. Add Contact\n");
		printf("2. Search Contact\n");
		printf("3. Edit Contact\n");
		printf("4. Add into existing contact\n");
		printf("5. Delete Contact\n");
		printf("6. List Contacts\n");
		printf("Please select an option: ");
}

Status menu(AddressBook* address_book)
{
		int option, opt, sr_no;
		do
		{
				main_menu();
				option = get_option(NUM, "");
				__fpurge(stdin);
				if ((address_book-> count == 0) && (option != e_add_contact) && (option != e_exit))
				{
						get_option(NONE, "No entries found!! Would you like to add? Use Add Contacts\n");
						continue;
				}
				switch (option)
				{
						case e_add_contact:
								/* Add your implementation to call add_contacts function here */
								//Create new contact info list
								address_book -> list = malloc(sizeof(ContactInfo));
								for (int i = 0; i < PHONE_NUMBER_COUNT; i++)
								{
										strcpy(address_book -> list -> phone_numbers[i], "");
								}
								add_contacts(address_book);
								return save_prompt(address_book);
								break;
						case e_search_contact:
								search_contact(address_book);
								break;
						case e_edit_contact:
								edit_contact(address_book);
								break;
						case e_add_into_existing:
								if ((sr_no = search_contact(address_book)) == e_fail)
										printf("Unable to edit!\n");
								else
								{
										search_serial_num(address_book, "**", sr_no);
										printf("\nWhat would you like to add in this contact?\n");
										printf("1. Phone number\n");
										printf("2. Email Address\n");
										printf("Choose an option: ");
										__fpurge(stdin);
										scanf("%d", &opt);
										if (opt == 1)
												add_more_numbers(address_book);
										else if (opt == 2)
												add_more_emails(address_book);
										else
												printf("Invalid option\n");
								}
								break;
						case e_delete_contact:
								delete_contact(address_book);
								break;
						case e_list_contacts:
								/* Add your implementation to call list_contacts function here */
								list_contacts(address_book);
								break;
						case e_exit:
						case e_no_opt:
								break;
						default://Invalid option
								printf("Invalid option!\n\n");
				}
		} while (option != e_exit);
		return e_success;
}

Status add_contacts(AddressBook* address_book)
{
		int option;
		bool_t name_added = 0;
		/* Add the functionality for adding contacts here */
		do
		{
				menu_header("What would you like to add?\n");
				show_menu();
				option = get_option(NUM, "");
				if (option != e_second_opt && !name_added)
				{
						get_option(NONE, "Name is not provided!! Please add a name!\n");
						continue;
				}
				switch (option)
				{
						case e_second_opt://Name
								validate_and_add_name(address_book);
								name_added = 1;
								break;
						case e_third_opt://Phone Number
								validate_and_add_ph_number(address_book);
								break;
						case e_fourth_opt://Email address
								validate_and_add_email_addr(address_book);
								break;
						case e_fifth_opt://Back
								break;
						case e_no_opt://Just enter
								break;
						default://Invalid option
								printf("Invalid option!!\n");
				}
		} while (option != e_fifth_opt);
		return e_success;
}

int search_contact(AddressBook* address_book)
{
		/* Add the functionality for search contacts here */
		int option, sr_no, ret;
		char str[EMAIL_ID_LEN];
		menu_header("By which of the following would you like to search?\n\n0. Serial Number");
		show_menu();
		option = get_option(NUM, "");
		__fpurge(stdin);
		switch (option)
		{
				case e_first_opt://Sr. No.
						printf("Enter serial number: ");
						scanf("%d", &sr_no);
						if ((ret = search_serial_num(address_book, "**", sr_no)) < 1)
						{
								printf("Serial number not found!\n\n");
								sleep(2);
								return e_fail;
						}
						printf("\n-------Search Result--------\n");
						printf("Serial Number : %d\n", ret);
						display_contact(address_book);
						sleep(3);
						break;
				case e_second_opt://Name
						printf("Enter Name to be searched: ");
						scanf("%[^\n]", str);
						if (validate_name(str))
						{
								if ((ret = search_serial_num(address_book, str, 0)) > 0)
								{
										printf("\n-------Search Result--------\n");
										printf("Serial Number : %d\n", ret);
										display_contact(address_book);
										sleep(3);
								}
								else
								{
										printf("Name not found!\n\n");
										sleep(2);
										return e_fail;
								}
						}
						else
						{
								printf("Invalid name!\n");
								return e_fail;
						}
						break;
				case e_third_opt://Phone Number
						printf("Enter Phone number to be searched: ");
						scanf("%[^\n]", str);
						if (validate_ph_no(str))
						{
								if ((ret = search_serial_num(address_book, str, 0)) >  0)
								{
										printf("\n-------Search Result--------\n");
										printf("Serial Number : %d\n", ret);
										display_contact(address_book);
										sleep(3);
								}
								else
								{
										printf("Phone number not found!\n\n");
										sleep(2);
										return e_fail;
								}
						}
						else
						{
								printf("Invalid phone number!\n");
								sleep(2);
								return e_fail;
						}
						break;
				case e_fourth_opt://Email address
						printf("Enter Email Address to be searched: ");
						scanf("%[^\n]", str);
						if (validate_email(str))
						{
								if ((ret = search_serial_num(address_book, str, 0) > 0))
								{
										printf("\n-------Search Result--------\n");
										printf("Serial Number : %d\n", ret);
										display_contact(address_book);
										sleep(3);
								}
								else
								{
										printf("Email address not found!\n\n");
										sleep(2);
										return e_fail;
								}
						}
						else
						{
								printf("Invalid email Id!\n");
								sleep(2);
								return e_fail;
						}
						break;
				case e_fifth_opt://Back
						return e_fail;
						break;
				case e_no_opt://Just enter
						return e_fail;
						break;
				default://Invalid option
						printf("Invalid option!!\n");
		}
		return ret;
}

Status edit_contact(AddressBook* address_book)
{
		/* Add the functionality for edit contacts here */
		int option;
		char str[EMAIL_ID_LEN];
		do
		{
				menu_header("Which of the following would you like to edit?\n");
				show_menu();
				option = get_option(NUM, "");
				__fpurge(stdin);
				switch (option)
				{
						case e_second_opt://Name
								printf("Enter the name to be edited: ");
								scanf("%[^\n]", str);
								if (!validate_name(str))
								{
										printf("Invalid name!\n");
										continue;
								}
								if (search_serial_num(address_book, str, 0) > 0)
								{
										printf("Name %s is found in the Addressbook\n", str);
										printf("Enter new name: ");
										__fpurge(stdin);
										scanf("%[^\n]", str);
										if (validate_name(str))
												edit_content(address_book, str);
										else
												printf("Unable to edit!!\n");
								}
								else
										printf("Name not found!\nUnable to edit the given name!!\n");
								break;
						case e_third_opt://Phone Number
								printf("Enter the phone no. to be edited: ");
								scanf("%[^\n]", str);
								if (!validate_ph_no(str))
								{
										printf("Invalid phone number!\n");
										continue;
								}
								if (search_serial_num(address_book, str, 0) > 0)
								{
										printf("Phone no. %s is found in the Addressbook\n", str);
										printf("Enter new phone no: ");
										__fpurge(stdin);
										scanf("%[^\n]", str);
										if (validate_ph_no(str))
												edit_content(address_book, str);
										else
												printf("Unable to edit!!\n");
								}
								else
										printf("Name not found!\nUnable to edit the given name!!\n");
								break;
						case e_fourth_opt://Email address
								printf("Enter the email address to be edited: ");
								scanf("%[^\n]", str);
								if (!validate_email(str))
								{
										printf("Invalid email Id!\n");
										continue;
								}
								if (search_serial_num(address_book, str, 0) > 0)
								{
										printf("Email Id %s is found in the Addressbook\n", str);
										printf("Enter new email id: ");
										__fpurge(stdin);
										scanf("%[^\n]", str);
										if (validate_email(str))
												edit_content(address_book, str);
										else
												printf("Unable to edit!!\n");
								}
								else
										printf("Email Id not found!\nUnable to edit the given email Id!!\n");
								break;
						case e_fifth_opt://Back
								break;
						case e_no_opt://Just enter
								break;
						default://Invalid option
								printf("Invalid option!!\n");
				}
		} while (option != e_fifth_opt);
		return e_success;
}

Status delete_contact(AddressBook* address_book)
{
		/* Add the functionality for delete contacts here */
		int option, sr_no;
		char str[EMAIL_ID_LEN];
		do
		{
				menu_header("By which of the following would you like to delete?\n\n0. Serial Number");
				show_menu();
				option = get_option(NUM, "");
				__fpurge(stdin);
				switch (option)
				{
						case e_first_opt://Sr. No.
								printf("Enter the serial no: ");
								scanf("%d", &sr_no);
								if (search_serial_num(address_book, "**", sr_no) > 0)
								{
										printf("Sr No. %d is found in the Addressbook\nDeleteing the entire contact..\n", sr_no);
										delete_line(address_book);
								}
								else
										printf("Serial no. not found!\nUnable to delete the given name!!\n");
								break;
						case e_second_opt://Name
								printf("Enter the name to be deleted: ");
								scanf("%[^\n]", str);
								if (search_serial_num(address_book, str, 0) > 0)
								{
										printf("Name %s is found in the Addressbook\nDeleteing the entire contact..\n", str);
										delete_line(address_book);
								}
								else
										printf("Name not found!\nUnable to delete the given name!!\n");
								break;
						case e_third_opt://Phone Number
								printf("Enter the phone no. to be deleted: ");
								scanf("%[^\n]", str);
								if (search_serial_num(address_book, str, 0) > 0)
								{
										printf("Phone no. %s is found in the Addressbook\nDeleteing the entire contact..\n", str);
										edit_content(address_book, "");
								}
								else
										printf("Phone no. not found!\nUnable to delete the given phone number!!\n");
								break;
						case e_fourth_opt://Email address
								printf("Enter the email address to be deleted: ");
								scanf("%[^\n]", str);
								if (search_serial_num(address_book, str, 0) > 0)
								{
										printf("Email Id %s is found in the Addressbook\nDeleteing the entire contact..\n", str);
										edit_content(address_book, "");
								}
								else
										printf("Email Id not found!\nUnable to delete the given email Id!!\n");
								break;
						case e_fifth_opt://Back
								break;
						case e_no_opt://Just enter
								break;
						default://Invalid option
								printf("Invalid option!!\n");
				}
		} while (option != e_fifth_opt);
		return e_success;
}

void show_menu(void)
{
		printf("1. Name\n");
		printf("2. Phone Number\n");
		printf("3. Email Address\n");
		printf("4. Back\n\n");
		printf("Please select an option: ");
}

Status validate_and_add_name(AddressBook* address_book)
{
		//Iterator 'i', name string, flag to check validity
		char name[NAME_LEN] = {'\0'};
		int i = 0;
		do
		{
				__fpurge(stdin);
				printf("Enter the Name: ");
				scanf("%32[^\n]", name);//Can take only NAME_LEN - 1 + 1 NUL character 
				bool_t validity = validate_name(name);
				if (validity)
				{
						printf("Name verified!\n");
						//Name validated. Add name and increment 'i'
						strcpy(address_book -> list -> name[i++], name);
						if (search_serial_num(address_book, name, 0) > 0)
						{
								printf("This name already exists in the Address Book!\nDuplicate name not allowed\n");
								return e_fail;
						}
				}
				else
				{
						printf("Invalid name!\nOnly alphabets and spaces are allowed!\n\n");
						continue;
				}
		} while (i < NAME_COUNT);//Can enter only NAME_COUNT no. of names
		return e_success;
}

Status validate_and_add_ph_number(AddressBook* address_book)
{
		//Iterator 'i', ph_no string, flag to check validity
		char ph_no[EMAIL_ID_LEN] = {'\0'}, add_extra = 'y';
		int i = 0;
		do
		{
				__fpurge(stdin);
				printf("Enter Phone Number %d: ", i + 1);
				scanf("%[^\n]", ph_no);//Only 10 digit phone no. + 1 NUL character is allowed 
				bool_t validity = validate_ph_no(ph_no);
				if (validity)
				{
						printf("Valid Phone number!\n");
						if (search_serial_num(address_book, ph_no, 0) > 0 || find_current_duplicate(address_book, ph_no, 1, i))
						{
								printf("This phone number already exists in the Address Book!\nDuplicate phone number not allowed\n");
								continue;
						}
						//Phone number validated. Add ph_no and increment 'i'
						strcpy(address_book -> list -> phone_numbers[i++], ph_no);
						if (i != PHONE_NUMBER_COUNT)
						{
								__fpurge(stdin);
								printf("Do you want to add more phone numbers? (%d more allowed) [y / n]: ", PHONE_NUMBER_COUNT - i);
								scanf("\n%c", &add_extra);
						}
				}
				else
				{
						printf("Invalid phone number!\nPlease enter 10-digit phone number\n\n");
						continue;
				}
		} while (i < PHONE_NUMBER_COUNT && add_extra == 'y');//Can enter only PHONE_NUMBER_COUNT no. of numbers
		return e_success;
}

Status validate_and_add_email_addr(AddressBook* address_book)
{
		//Iterator 'i', email string, flag to check validity
		char email[EMAIL_ID_LEN] = {'\0'}, add_extra = 'y';
		int i = 0;
		do
		{
				__fpurge(stdin);
				printf("Enter Email Address %d: ", i + 1);
				scanf("%32[^\n]", email);//Can take only EMAIL_ID_LEN - 1 + 1 NUL character 
				bool_t validity = validate_email(email);
				if (validity)
				{
						printf("Valid Email Id!\n");
						if (search_serial_num(address_book, email, 0) > 0 || find_current_duplicate(address_book, email, 0, i))
						{
								printf("This email address already exists in the Address Book!\nDuplicate email address not allowed\n");
								continue;
						}
						//Phone number validated. Add ph_no and increment 'i'
						strcpy(address_book -> list -> email_addresses[i++], email);
						if (i != EMAIL_ID_COUNT)
						{
								__fpurge(stdin);
								printf("Do you want to add more email Ids? (%d more allowed) [y / n]: ", PHONE_NUMBER_COUNT - i);
								scanf("\n%c", &add_extra);
						}
				}
				else
				{
						printf("Invalid email address!\nPlease enter a valid email id. eg:- someone12@something.com\n\n");
						continue;
				}
		} while (i < EMAIL_ID_COUNT && add_extra == 'y');//Can enter only EMAIL_ID_COUNT no. of emails
		return e_success;
}

bool_t validate_name(const char* name)
{
		printf("Verifying given name..\n");
		int j = 0;//Iterate through name string for validation
		while (j < strlen(name))
		{
				//Check for charcters other than space & alphabets
				if (name[j] != ' ' && !isalpha(name[j]))
				{
						printf("Invalid name provided!\nName should consists of alphabets only! Please provide a valid name\n\n");
						return 0;
				}
				j++;
		}
		return 1;
}

bool_t validate_ph_no(const char* ph_no)
{
		printf("Verifying given number..\n");
		int j = 0;//Iterate through ph_no string for validation
		int num_len = strlen(ph_no);
		//printf("Num len = %d\n", num_len);
		if (num_len != 10)
				return 0;
		while (j < num_len)
		{
				//Check for charcters other than digits
				if (!isdigit(ph_no[j++]))
				{
						printf("Invalid phone number provided!\nPhone number should consists of exactly 10 digits! Please provide a valid phone number\n\n");
						return 0;
				}
		}
		return 1;
}

bool_t validate_email(const char* email)
{
		printf("Verifying given email Id..\n");
		int j = 0,//Iterate through ph_no string for validation
                     at_rate_index = 0,//Index of '@'
                     at_rate_cnt = 0,//Count of '@'
                     dot_index = 0,//Index of '.'
                     dot_cnt = 0;//Count of '.'
                while (j < strlen(email))//Iterate over length of email string
                {
						//printf("character = %c, j = %d\n", email[j], j);
						//Check if '@' + some alphabrets + '.' + some alphabets
                        if (isalnum(email[j]) || email[j] == '@' || email[j] == '.' || email[j] == '_' || email[j] == '-' || email[j] == '+')
                        {
                                if (email[j] == '@')
                                {
										//printf("@ found at index %d\n", j);
                                        at_rate_index = j, at_rate_cnt++;
                                }
								if (isalpha(email[j]) || email[j] == '.')
								{
										if (email[j] == '.')
										{
												//printf("Dot found at index %d\n", j);
												dot_index = j, dot_cnt++;
										}
								}
                                if (j > dot_index && (at_rate_index < dot_index) && at_rate_cnt && dot_cnt && !isalpha(email[j]))
                                {
										return 0;
                                }
                        }
                        else
								return 0;
                        j++;
                }
				//printf("@ index = %d\n", at_rate_index);
				//printf("dot index = %d\n", dot_index);
				if ((at_rate_index != 0) && dot_index && (at_rate_cnt == 1) && (at_rate_index + 1 != dot_index) && (dot_index != j - 1))
                        return 1;
				return 0;
}

void display_contact(AddressBook* address_book)
{
		char ch, str[EMAIL_ID_LEN];	
        while ((ch = fgetc(address_book -> fp)) != '\n')
        {
                fseek(address_book -> fp, -2L, SEEK_CUR);
                if (ftell(address_book -> fp) == 0)
                        break;
        }
		int i = 0;
        //printf("File offset of beginning of contact: %ld\n", ftell(fp));
		while ((ch = fgetc(address_book -> fp)) != ',')
		{
				str[i++] = ch;
		}
		str[i] = '\0';
		printf("Name          : %s\n", str);
		for (int k = 1; k <= PHONE_NUMBER_COUNT; k++)
		{
				i = 0;
				while ((ch = fgetc(address_book -> fp)) != ',')
				{
						str[i++] = ch;
				}
				str[i] = '\0';
				if (strcmp(str, ""))
						printf("Contact No. %d : %s\n", k, str);
		}
		for (int k = 1; k <= EMAIL_ID_COUNT; k++)
		{
				i = 0;
				while ((ch = fgetc(address_book -> fp)) != '\n')
				{
						fseek(address_book -> fp, -1L, SEEK_CUR);
						if ((ch = fgetc(address_book -> fp)) == ',')
								break;
						else
								str[i++] = ch;
				}
				str[i] = '\0';
				if (strcmp(str, ""))
						printf("Email Id %d    : %s\n", k, str);
		}
}

void list_contacts(AddressBook* address_book)
{
		fflush(stdout);
		system("clear");
		int CONTACTS_PER_PAGE = 4;
		rewind(address_book -> fp);
        int sr_no = 1;
        rewind(address_book -> fp);
		char opt;
        while (1)
        {
				printf("--------------------------------------\n");
				printf("Serial Number : %d\n", sr_no);
				display_contact(address_book);
				if (sr_no % CONTACTS_PER_PAGE == 0 || sr_no == address_book -> count)
				{
						printf("--------------------------------------\n");
						printf("\n'p': previous page\
										\n'n': Next Page\
										\n'q': Quit\
										\nPress an option key: ");
						__fpurge(stdin);
						scanf("\n%c", &opt);
						fflush(stdout);
						system("clear");
						switch (opt)
						{
								case 'n'://Next
										if (sr_no == address_book -> count)
									    {
												while (sr_no % CONTACTS_PER_PAGE != 1)
														sr_no--;
												search_serial_num(address_book, "**", sr_no);
												sr_no--;
										}
										break;
								case 'p'://Previous
										if (sr_no == CONTACTS_PER_PAGE)
												search_serial_num(address_book, "**", sr_no = 1);
										else if (sr_no > CONTACTS_PER_PAGE && sr_no != address_book -> count)
												search_serial_num(address_book, "**", sr_no -= (2 * CONTACTS_PER_PAGE - 1));
										else if (sr_no == address_book -> count)
										{
												while (sr_no % CONTACTS_PER_PAGE != 1)
														sr_no--;
												sr_no -= CONTACTS_PER_PAGE;
												search_serial_num(address_book, "**", sr_no);
										}
										sr_no--;
										break;
								case 'q'://Quit
										return;
                                default://Nothing
										while (sr_no % CONTACTS_PER_PAGE != 1)
												sr_no--;
										search_serial_num(address_book, "**", sr_no);
										sr_no--;
										break;
						}
				}
				sr_no++;
        }
}

void delete_line(AddressBook* address_book)
{
		// Delete contact 
		char ch;
		while ((ch = fgetc(address_book -> fp)) != NEXT_ENTRY)
		{
				fseek(address_book -> fp, -2L, SEEK_CUR);
				if (ftell(address_book -> fp) == 0)
						break;
		}
		//printf("File offset of beginning of contact: %ld\n", ftell(address_book -> fp));
		long line_del = ftell(address_book -> fp);
		fseek(address_book -> fp, 0L, SEEK_END);
		long eof = ftell(address_book -> fp);
		fseek(address_book -> fp, line_del, SEEK_SET);
		//printf("File offset at line to be deleted: %ld\n", ftell(address_book -> fp));
		fscanf(address_book -> fp, "%c", &ch);
		fseek(address_book -> fp, -1L, SEEK_CUR);
		//printf("Current entry's character begins with: %c\n", ch);
		//Find the next entry
		while ((ch = fgetc(address_book -> fp)) != NEXT_ENTRY);
		long next_entry = ftell(address_book -> fp);
		long cpy_size2 = eof - next_entry;
		//printf("Next entry found at %ld\n", next_entry);
		fscanf(address_book -> fp, "%c", &ch);
		fseek(address_book -> fp, -1L, SEEK_CUR);
		//printf("Next entry's character begins with: %c\n", ch);
		char* cpy_part2 = malloc((cpy_size2 + 1) * sizeof(char));
		//Copy the initial part
		rewind(address_book -> fp);
		char* cpy_part1 = malloc((line_del + 1) * sizeof(char));
		fread(cpy_part1, line_del, 1, address_book -> fp);
		//printf("File offset after copying part 1: %ld\n", ftell(address_book -> fp));
		//printf("File offset before copying part 2: %ld\n", ftell(address_book -> fp));
		fseek(address_book -> fp, next_entry, SEEK_SET);
		fread(cpy_part2, cpy_size2, 1, address_book -> fp);
		//printf("File offset after copying part 2: %ld\n", ftell(address_book -> fp));
		FILE* fp_reopen;
		//Paste both the parts after reopening file in "w" mode
		if ((fp_reopen = freopen(DEFAULT_FILE, "w", address_book -> fp)) == NULL)
		{
				perror("open");
				return;
		}
		address_book -> fp = fp_reopen;
		//Write part 1
		fwrite(cpy_part1, line_del, 1, address_book -> fp);
		//Write part 2
		fwrite(cpy_part2, cpy_size2, 1, address_book -> fp);
		free(cpy_part1);
		free(cpy_part2);
		//Decrement address book count
		--(address_book -> count);
		//Change back to "r" mode
		if ((fp_reopen = freopen(DEFAULT_FILE, "r", address_book -> fp)) == NULL)
		{
				perror("open");
				return;
		}
		address_book -> fp = fp_reopen;
}

void edit_content(AddressBook* address_book, char* new_str)
{
		// Edit contact 
		char ch;
		long line_ed = ftell(address_book -> fp);
		fseek(address_book -> fp, 0L, SEEK_END);
		long eof = ftell(address_book -> fp);
		fseek(address_book -> fp, line_ed, SEEK_SET);
		//printf("File offset at line to be edited: %ld\n", ftell(address_book -> fp));
		fscanf(address_book -> fp, "%c", &ch);
		fseek(address_book -> fp, -1L, SEEK_CUR);
		//printf("Current entry's character begins with: %c\n", ch);
		//Find the next entry
		while ((ch = fgetc(address_book -> fp)) != NEXT_ENTRY)
		{
				fseek(address_book -> fp, -1L, SEEK_CUR);
				if ((ch = fgetc(address_book -> fp)) == FIELD_DELIMITER)
				{
						break;
				}
		}
		fseek(address_book -> fp, -1L, SEEK_CUR);
		long next_field = ftell(address_book -> fp);
		long cpy_size2 = eof - next_field;
		//printf("Next entry found at %ld\n", next_field);
		fscanf(address_book -> fp, "%c", &ch);
		fseek(address_book -> fp, -1L, SEEK_CUR);
		//printf("Next entry's character begins with: %c\n", ch);
		char* cpy_part2 = malloc((cpy_size2 + 1) * sizeof(char));
		//Copy the initial part
		rewind(address_book -> fp);
		char* cpy_part1 = malloc((line_ed + 1) * sizeof(char));
		fread(cpy_part1, line_ed, 1, address_book -> fp);
		//printf("File offset after copying part 1: %ld\n", ftell(address_book -> fp));
		//printf("File offset before copying part 2: %ld\n", ftell(address_book -> fp));
		fseek(address_book -> fp, next_field, SEEK_SET);
		fread(cpy_part2, cpy_size2, 1, address_book -> fp);
		//printf("File offset after copying part 2: %ld\n", ftell(address_book -> fp));
		FILE* fp_reopen;
		//Paste both the parts after reopening file in "w" mode
		if ((fp_reopen = freopen(DEFAULT_FILE, "w", address_book -> fp)) == NULL)
		{
				perror("open");
				return;
		}
		address_book -> fp = fp_reopen;
		//Write part 1
		fwrite(cpy_part1, line_ed, 1, address_book -> fp);
		//Add new string
		fwrite(new_str, strlen(new_str), 1, address_book -> fp);
		//Write part 2
		fwrite(cpy_part2, cpy_size2, 1, address_book -> fp);
		free(cpy_part1);
		free(cpy_part2);
		//Change back to "r" mode
		if ((fp_reopen = freopen(DEFAULT_FILE, "r", address_book -> fp)) == NULL)
		{
				perror("open");
				return;
		}
		address_book -> fp = fp_reopen;
}

bool_t find_current_duplicate(AddressBook* address_book, const char* str, int num_or_email, int index)
{
		for (int i = 0; i < index; i++)
		{
				if (num_or_email)//Compare numbers
				{
						if (!strcmp(address_book -> list -> phone_numbers[i], str))
								return 1;
				}
				else//Compare emails
				{
						if (!strcmp(address_book -> list -> email_addresses[i], str))
								return 1;
				}
		}
		return 0;
}

Status add_more_numbers(AddressBook* address_book)
{
		char ch, comma_count = 0;
		//Go to first comma before numbers
		while((ch = fgetc(address_book -> fp)))
		{
				if (ch == FIELD_DELIMITER)
						comma_count++;
				if (comma_count == 1)
						break;
		}
		fseek(address_book -> fp, -1L, SEEK_CUR);
		while ((ch = fgetc(address_book -> fp)) != EOF)
		{
				fseek(address_book -> fp, -1L, SEEK_CUR);
				if ((ch = fgetc(address_book -> fp)) == FIELD_DELIMITER)
				{
						comma_count++;
						if ((ch = fgetc(address_book -> fp)) == FIELD_DELIMITER || ch == 0)
						{
								fseek(address_book -> fp, -1L, SEEK_CUR);
								break;
						}
				}
		}
		//printf("Comma count = %d\n", comma_count);
		fseek(address_book -> fp, -1L, SEEK_CUR);
		if (comma_count > 6)
		{
				printf("\nCannot add any more phone numbers!\n");
				sleep(2);
				return e_fail;
		}
		char new_str[NUMBER_LEN];
		printf("Enter phone number: ");
		__fpurge(stdin);
		scanf("%[^\n]", new_str);
		if (!validate_ph_no(new_str))
		{
				printf("\nInvalid phone number!\n");
				sleep(2);
				return e_fail;
		}
		printf("Valid phone number!\n");
		long temp = ftell(address_book -> fp);
		if (search_serial_num(address_book, new_str, 0) > 0)
		{
				printf("This phone number already exists in the Address Book!\nDuplicate name not allowed\n");
				sleep(2);
				return e_fail;
		}
		fseek(address_book -> fp, temp, SEEK_SET);
		//printf("%ld\n", ftell(address_book -> fp));
		ch = fgetc(address_book -> fp);
		//printf("ch = %c\n", ch);
		edit_content(address_book, new_str);
		printf("\nSuccessfully added this phone no. into this contact!\n");
		sleep(2);
		return e_success;
}

Status add_more_emails(AddressBook* address_book)
{
		char ch, comma_count = 0;
		//Go to first comma before emails
		while((ch = fgetc(address_book -> fp)))
		{
				if (ch == FIELD_DELIMITER)
						comma_count++;
				if (comma_count == 6)
						break;
		}
		fseek(address_book -> fp, -1L, SEEK_CUR);
		while ((ch = fgetc(address_book -> fp)) != EOF)
		{
				fseek(address_book -> fp, -1L, SEEK_CUR);
				if ((ch = fgetc(address_book -> fp)) == FIELD_DELIMITER)
				{
						comma_count++;
						if ((ch = fgetc(address_book -> fp)) == FIELD_DELIMITER || ch == NEXT_ENTRY)
						{
								fseek(address_book -> fp, -1L, SEEK_CUR);
								break;
						}
				}
		}
		//printf("Comma count = %d\n", comma_count);
		if (ch == EOF || comma_count > 11)
		{
				printf("\nCannot add any more contacts!\n");
				sleep(2);
				return e_fail;
		}
		char new_str[EMAIL_ID_LEN];
		printf("Enter Email Id: ");
		__fpurge(stdin);
		scanf("%[^\n]", new_str);
		if (!validate_email(new_str))
		{
				printf("\nInvalid email Id!\n");
				sleep(2);
				return e_fail;
		}
		printf("\nValid email Id!\n");
		sleep(2);
		long temp = ftell(address_book -> fp);
		if (search_serial_num(address_book, new_str, 0) > 0)
		{
				printf("This email Id already exists in the Address Book!\nDuplicate name not allowed\n");
				sleep(2);
				return e_fail;
		}
		fseek(address_book -> fp, temp, SEEK_SET);
		fseek(address_book -> fp, -1L, SEEK_CUR);
		if (ch == NEXT_ENTRY)
		{
				fseek(address_book -> fp, 1L, SEEK_CUR);
				edit_content(address_book, new_str);
				return e_success;
		}
		//printf("%ld\n", ftell(address_book -> fp));
		ch = fgetc(address_book -> fp);
		//printf("ch = %c\n", ch);
		edit_content(address_book, new_str);
		printf("\nSuccessfully added this email Id into this contact!\n");
		sleep(2);
		return e_success;
}

