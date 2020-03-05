#ifndef ABK_MENUS_H
#define ABK_MENUS_H

#include "address_book.h"

#define	NONE							0
#define	NUM								1
#define	CHAR							2

#define WINDOW_SIZE						5

void menu_header(const char* str);
void main_menu(void);
void show_menu(void);
int search_serial_num(AddressBook* address_book, char* search_str, int sr_no);
Status menu(AddressBook* address_book);
Status save_prompt(AddressBook* address_book);
Status add_contacts(AddressBook* address_book);
int search_contact(AddressBook* address_book);
Status edit_contact(AddressBook* address_book);
Status delete_contact(AddressBook* address_book);
Status validate_and_add_name(AddressBook* address_book);
Status validate_and_add_ph_number(AddressBook* address_book);
Status validate_and_add_email_addr(AddressBook* address_book);
bool_t validate_name(const char* name);
bool_t validate_ph_no(const char* ph_no);
bool_t validate_email(const char* email);
void display_contact(AddressBook* address_book);
void list_contacts(AddressBook* address_book);
void delete_line(AddressBook* address_book);
void edit_content(AddressBook* address_book, char* new_str);
bool_t find_current_duplicate(AddressBook* address_book, const char* str, int num_or_email, int index);
Status add_more_numbers(AddressBook* address_book);
Status add_more_emails(AddressBook* address_book);

#endif

