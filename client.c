#include <ctype.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helpers.h"
#include "parson.h"
#include "requests.h"

void register_client(char *host_ip, int sockfd, char *cookie)
{
	char username[100], password[100];

	// citirea username-ului de la tastatura
	printf("username=");
	fgets(username, 100, stdin);
	username[strlen(username) - 1] = '\0';

	// citirea parolei de la tastatura
	printf("password=");
	fgets(password, 100, stdin);
	password[strlen(password) - 1] = '\0';

	// verifica daca utilizatorul este deja inregistrat
	if (cookie[0] != '\0') {
		printf("ERROR: The user is already registered!\n");
		return;
	}

	// verificare daca username-ul contine spatii
	if (strchr(username, ' ') != NULL) {
		printf("ERROR: The username cannot contain spaces!\n");
		return;
	}
	// verificare daca username-ul este gol
	else if (strlen(username) == 0) {
		printf("ERROR: The username is empty!\n");
		return;
	}

	// verificare daca parola contine spatii
	if (strchr(password, ' ') != NULL) {
		printf("ERROR: The password cannot contain spaces!\n");
		return;
	}
	// verificare daca parola este goala
	else if (strlen(password) == 0) {
		printf("ERROR: The password is empty!\n");
		return;
	}

	// crearea unui obiect JSON cu username si parola
	JSON_Value *root_value = json_value_init_object();
	JSON_Object *root_object = json_value_get_object(root_value);

	json_object_set_string(root_object, "username", username);
	json_object_set_string(root_object, "password", password);

	// serializarea obiectului JSON
	char *serialized_string = json_serialize_to_string_pretty(root_value);

	// crearea mesajului de cerere POST
	char *message = compute_post_request(host_ip, "/api/v1/tema/auth/register", "application/json", &serialized_string,
										 1, NULL, 0, NULL);

	// trimiterea cererii la server
	send_to_server(sockfd, message);

	// primirea raspunsului de la server
	char *response = receive_from_server(sockfd);

	// verificare daca username-ul este deja luat
	if (strstr(response, "error") != NULL) {
		printf("ERROR: The username is already taken!\n");
	} else {
		printf("SUCCESS: The user was successfully registered!\n");
	}

	// eliberarea memoriei alocate
	free(serialized_string);
	free(message);
	free(response);
	json_value_free(root_value);

	return;
}

void login_client(char *host_ip, int sockfd, char *cookie)
{
	char username[100], password[100];

	// citirea username-ului de la tastatura
	printf("username=");
	fgets(username, 100, stdin);
	username[strlen(username) - 1] = '\0';

	// citirea parolei de la tastatura
	printf("password=");
	fgets(password, 100, stdin);
	password[strlen(password) - 1] = '\0';

	// verifica daca utilizatorul este deja logat
	if (cookie[0] != '\0') {
		printf("ERROR: The user is already logged in!\n");
		return;
	}

	// verificare daca username-ul contine spatii
	if (strchr(username, ' ') != NULL) {
		printf("ERROR: The username cannot contain spaces!\n");
		return;
	}
	// verificare daca username-ul este gol
	else if (strlen(username) == 0) {
		printf("ERROR: The username is empty!\n");
		return;
	}

	// verificare daca parola contine spatii
	if (strchr(password, ' ') != NULL) {
		printf("ERROR: The password cannot contain spaces!\n");
		return;
	}
	// verificare daca parola este goala
	else if (strlen(password) == 0) {
		printf("ERROR: The password is empty!\n");
		return;
	}

	// crearea unui obiect JSON cu username si parola
	JSON_Value *root_value = json_value_init_object();
	JSON_Object *root_object = json_value_get_object(root_value);

	json_object_set_string(root_object, "username", username);
	json_object_set_string(root_object, "password", password);

	// serializarea obiectului JSON
	char *serialized_string = json_serialize_to_string_pretty(root_value);

	// crearea mesajului de cerere POST
	char *message = compute_post_request(host_ip, "/api/v1/tema/auth/login", "application/json", &serialized_string, 1,
										 NULL, 0, NULL);

	// trimiterea cererii la server
	send_to_server(sockfd, message);

	// primirea raspunsului de la server
	char *response = receive_from_server(sockfd);

	// verificare daca username-ul sau parola sunt incorecte
	if (strstr(response, "error") != NULL) {
		printf("ERROR: The username or password is incorrect!\n");
	} else {
		// extragerea cookie-ului din raspunsul serverului
		char *cookie_start = strstr(response, "Set-Cookie: ");
		char *cookie_end = strstr(cookie_start, ";");

		strncpy(cookie, cookie_start + 12, cookie_end - cookie_start - 12);

		printf("SUCCESS: The user was successfully logged in!\n");
	}

	// eliberarea memoriei alocate
	free(serialized_string);
	free(message);
	free(response);
	json_value_free(root_value);

	return;
}

void enter_library(char *host_ip, int sockfd, char *cookie, char *token)
{
	// verifica daca utilizatorul a intrat deja in biblioteca
	if (token[0] != '\0') {
		printf("ERROR: The user has already entered the library!\n");
		return;
	}

	// crearea mesajului de cerere GET
	char *message = compute_get_request(host_ip, "/api/v1/tema/library/access", NULL, &cookie, 1, NULL);

	// trimiterea cererii la server
	send_to_server(sockfd, message);

	// primirea raspunsului de la server
	char *response = receive_from_server(sockfd);

	// verificare daca credentialele se potrivesc
	if (strstr(response, "error") != NULL) {
		printf("ERROR: The user's credentials dont match!\n");
	} else {
		// extragerea token-ului din raspunsul serverului
		char *token_start = strstr(response, "token\":\"");
		char *token_end = strstr(token_start + 8, "\"");

		strncpy(token, token_start + 8, token_end - token_start - 8);
		printf("SUCCESS: The user has successfully entered the library!\n");
	}

	// eliberarea memoriei alocate
	free(message);
	free(response);

	return;
}

void get_books(char *host_ip, int sockfd, char *cookie, char *token)
{
	// verifica daca utilizatorul este logat
	if (cookie[0] == '\0') {
		printf("ERROR: The user is not logged in!\n");
		return;
	}

	// crearea mesajului de cerere GET
	char *message = compute_get_request(host_ip, "/api/v1/tema/library/books", NULL, &cookie, 1, token);

	// trimiterea cererii la server
	send_to_server(sockfd, message);

	// primirea raspunsului de la server
	char *response = receive_from_server(sockfd);

	// verificare daca utilizatorul are acces la biblioteca
	if (strstr(response, "error") != NULL) {
		printf("ERROR: The user doesn't have access to the library!\n");
	} else {
		// extragerea inceputului JSON-ului din raspunsul serverului
		char *json_start = strchr(response, '[');

		// parsarea raspunsului ca JSON
		JSON_Value *root_value = json_parse_string(json_start);
		if (root_value == NULL) {
			printf("ERROR: Could not parse response as JSON\n");
			free(message);
			free(response);

			return;
		}

		// obtinerea array-ului de carti din JSON
		JSON_Array *books = json_value_get_array(root_value);
		if (books == NULL) {
			printf("ERROR: Response does not contain a JSON array\n");
			free(message);
			free(response);

			return;
		}

		// afisarea cartilor
		printf("[\n");
		for (int i = 0; i < json_array_get_count(books); i++) {
			JSON_Object *book = json_array_get_object(books, i);

			printf("   {\n");
			printf("      \"id\": %d,\n", (int)json_object_get_number(book, "id"));
			printf("      \"title\": \"%s\"\n", json_object_get_string(book, "title"));
			if (i == json_array_get_count(books) - 1) {
				printf("   }\n");
			} else {
				printf("   },\n");
			}
		}
		printf("]\n");
	}

	// eliberarea memoriei alocate
	free(message);
	free(response);

	return;
}

void get_book(char *host_ip, int sockfd, char *cookie, char *token)
{
	// citirea id-ului cartii de la utilizator
	char id[100];
	printf("id=");
	fgets(id, 100, stdin);
	id[strlen(id) - 1] = '\0';

	// verifica daca utilizatorul este logat
	if (cookie[0] == '\0') {
		printf("ERROR: The user is not logged in!\n");
		return;
	}

	// verificare daca id-ul este valid
	if (strlen(id) == 0) {
		printf("ERROR: id is empty!\n");
		return;
	} else {
		for (int i = 0; i < strlen(id); i++) {
			if (!isdigit(id[i])) {
				printf("ERROR: id must be a number!\n");
				return;
			}
		}
	}

	// crearea URL-ului pentru cererea GET
	char *url = malloc(100);
	sprintf(url, "/api/v1/tema/library/books/%s", id);

	// crearea mesajului de cerere GET
	char *message = compute_get_request(host_ip, url, NULL, NULL, 0, token);

	// trimiterea cererii la server
	send_to_server(sockfd, message);

	// primirea raspunsului de la server
	char *response = receive_from_server(sockfd);

	// verificare daca utilizatorul are acces la bibilioteca sau daca cartea exista
	if (strstr(response, "error") != NULL) {
		if (strstr(response, "token") != NULL) {
			printf("ERROR: The user doesn't have access to the library!\n");
		} else if (strstr(response, "book") != NULL) {
			printf("ERROR: The book with id %s doesn't exist!\n", id);
		}
	} else {
		// extragerea inceputului JSON-ului din raspunsul serverului
		char *json_start = strchr(response, '{');

		// parsarea raspunsului ca JSON
		JSON_Value *root_value = json_parse_string(json_start);
		if (root_value == NULL) {
			printf("ERROR: Could not parse response as JSON\n");
			free(url);
			free(message);
			free(response);

			return;
		}

		// obtinerea obiectului carte din JSON
		JSON_Object *book = json_value_get_object(root_value);
		if (book == NULL) {
			printf("ERROR: Response does not contain a JSON object\n");
			free(url);
			free(message);
			free(response);

			return;
		}

		// afisarea cartii
		printf("{\n");
		printf("   \"id\": %d,\n", (int)json_object_get_number(book, "id"));
		printf("   \"title\": \"%s\",\n", json_object_get_string(book, "title"));
		printf("   \"author\": \"%s\",\n", json_object_get_string(book, "author"));
		printf("   \"publisher\": \"%s\",\n", json_object_get_string(book, "publisher"));
		printf("   \"genre\": \"%s\",\n", json_object_get_string(book, "genre"));
		printf("   \"page_count\": %d\n", (int)json_object_get_number(book, "page_count"));
		printf("}\n");
	}

	// eliberarea memoriei alocate
	free(url);
	free(message);
	free(response);

	return;
}

void add_book(char *host_ip, int sockfd, char *cookie, char *token)
{
	char title[100], author[100], genre[100], publisher[100], page_count[10];

	// citirea informatiilor despre carte de la utilizator
	printf("title=");
	fgets(title, 100, stdin);
	title[strlen(title) - 1] = '\0';
	printf("author=");
	fgets(author, 100, stdin);
	author[strlen(author) - 1] = '\0';
	printf("genre=");
	fgets(genre, 100, stdin);
	genre[strlen(genre) - 1] = '\0';
	printf("publisher=");
	fgets(publisher, 100, stdin);
	publisher[strlen(publisher) - 1] = '\0';
	printf("page_count=");
	fgets(page_count, 10, stdin);
	page_count[strlen(page_count) - 1] = '\0';

	// verifica daca utilizatorul este logat
	if (cookie[0] == '\0') {
		printf("ERROR: The user is not logged in!\n");
		return;
	}

	// verificare daca informatiile despre carte sunt valide
	if (strlen(title) == 0) {
		printf("ERROR: title is empty!\n");
		return;
	} else if (strlen(author) == 0) {
		printf("ERROR: author is empty!\n");
		return;
	} else if (strlen(genre) == 0) {
		printf("ERROR: genre is empty!\n");
		return;
	} else if (strlen(publisher) == 0) {
		printf("ERROR: publisher is empty!\n");
		return;
	} else if (strlen(page_count) == 0) {
		printf("ERROR: page_count is empty!\n");
		return;
	} else {
		for (int i = 0; i < strlen(page_count); i++) {
			if (!isdigit(page_count[i])) {
				printf("ERROR: page_count must be a number!\n");
				return;
			}
		}
	}

	// crearea unui obiect JSON cu informatiile despre carte
	JSON_Value *root_value = json_value_init_object();
	JSON_Object *root_object = json_value_get_object(root_value);

	json_object_set_string(root_object, "title", title);
	json_object_set_string(root_object, "author", author);
	json_object_set_string(root_object, "genre", genre);
	json_object_set_number(root_object, "page_count", atoi(page_count));
	json_object_set_string(root_object, "publisher", publisher);

	// serializarea obiectului JSON
	char *serialized_string = json_serialize_to_string_pretty(root_value);

	// crearea mesajului de cerere POST
	char *message = compute_post_request(host_ip, "/api/v1/tema/library/books", "application/json", &serialized_string,
										 1, NULL, 0, token);

	// trimiterea cererii la server
	send_to_server(sockfd, message);

	// primirea raspunsului de la server
	char *response = receive_from_server(sockfd);

	// verificare daca utilizatorul are acces la bibilioteca sau daca informatiile introduse sunt bune
	if (strstr(response, "error") != NULL) {
		if (strstr(response, "token") != NULL) {
			printf("ERROR: The user doesn't have access to the library!\n");
		} else {
			printf("ERROR: The book information doesn't respect the format!\n");
		}
	} else {
		printf("SUCCESS: The book was successfully added!\n");
	}

	// eliberarea memoriei alocate
	free(serialized_string);
	free(message);
	free(response);
	json_value_free(root_value);

	return;
}

void delete_book(char *host_ip, int sockfd, char *cookie, char *token)
{
	char id[10];

	// citirea id-ului cartii de la utilizator
	printf("id=");
	fgets(id, 10, stdin);
	id[strlen(id) - 1] = '\0';

	// verifica daca utilizatorul este logat
	if (cookie[0] == '\0') {
		printf("ERROR: The user is not logged in!\n");
		return;
	}

	// verificare daca id-ul este valid
	if (strlen(id) == 0) {
		printf("ERROR: id is empty!\n");
		return;
	} else {
		for (int i = 0; i < strlen(id); i++) {
			if (!isdigit(id[i])) {
				printf("ERROR: id must be a number!\n");
				return;
			}
		}
	}

	// crearea URL-ului pentru cererea DELETE
	char *url = malloc(100);
	sprintf(url, "/api/v1/tema/library/books/%s", id);

	// crearea mesajului de cerere DELETE
	char *message = compute_delete_request(host_ip, url, NULL, &cookie, 1, token);

	// trimiterea cererii la server
	send_to_server(sockfd, message);

	// primirea raspunsului de la server
	char *response = receive_from_server(sockfd);

	// verificare daca utilizatorul are acces la bibilioteca sau daca id-ul este valid
	if (strstr(response, "error") != NULL) {
		if (strstr(response, "token") != NULL) {
			printf("ERROR: The user doesn't have access to the library!\n");
		} else if (strstr(response, "book") != NULL) {
			printf("ERROR: The book with id %s doesn't exist!\n", id);
		}
	} else {
		printf("SUCCESS: The book with id %s was successfully deleted!\n", id);
	}

	// eliberarea memoriei alocate
	free(url);
	free(message);
	free(response);

	return;
}

void logout_client(char *host_ip, int sockfd, char *cookie, char *token)
{
	// crearea mesajului de cerere GET pentru logout
	char *message = compute_get_request(host_ip, "/api/v1/tema/auth/logout", NULL, &cookie, 1, token);

	// trimiterea cererii la server
	send_to_server(sockfd, message);

	// primirea raspunsului de la server
	char *response = receive_from_server(sockfd);

	// verificare daca utilizatorul este autentificat
	if (strstr(response, "error") != NULL) {
		printf("ERROR: The user is not currently logged in!\n");
	} else {
		// stergerea cookie-ului si a token-ului de acces
		memset(cookie, 0, 100);
		memset(token, 0, 300);
		printf("SUCCESS: The user was successfully logged out!\n");
	}

	// eliberarea memoriei alocate
	free(message);
	free(response);

	return;
}

int main()
{
	// IP-ul host-ului
	char host_ip[] = "34.246.184.49";
	// deschiderea conexiunii cu serverul
	int sockfd = open_connection(host_ip, 8080, AF_INET, SOCK_STREAM, 0);

	// alocarea memoriei pentru actiune, cookie si token
	char *action = malloc(15);
	char *cookie = malloc(100);
	char *token = malloc(300);

	// initializarea cookie-ului si a token-ului cu 0
	memset(cookie, 0, 100);
	memset(token, 0, 300);

	while (1) {
		// citirea actiunii de la utilizator
		fgets(action, 15, stdin);

		// daca actiunea este "exit", se iese din bucla
		if (strcmp(action, "exit\n") == 0) {
			break;
		} else {
			// deschiderea conexiunii cu serverul
			sockfd = open_connection(host_ip, 8080, AF_INET, SOCK_STREAM, 0);

			// executarea actiunii corespunzatoare
			if (strcmp(action, "register\n") == 0) {
				register_client(host_ip, sockfd, cookie);
			} else if (strcmp(action, "login\n") == 0) {
				login_client(host_ip, sockfd, cookie);
			} else if (strcmp(action, "enter_library\n") == 0) {
				enter_library(host_ip, sockfd, cookie, token);
			} else if (strcmp(action, "get_books\n") == 0) {
				get_books(host_ip, sockfd, cookie, token);
			} else if (strcmp(action, "get_book\n") == 0) {
				get_book(host_ip, sockfd, cookie, token);
			} else if (strcmp(action, "add_book\n") == 0) {
				add_book(host_ip, sockfd, cookie, token);
			} else if (strcmp(action, "delete_book\n") == 0) {
				delete_book(host_ip, sockfd, cookie, token);
			} else if (strcmp(action, "logout\n") == 0) {
				logout_client(host_ip, sockfd, cookie, token);
			} else {
				// daca actiunea nu este recunoscuta, se afiseaza un mesaj de eroare
				printf("Invalid command!\n");
			}

			// inchiderea conexiunii cu serverul
			close_connection(sockfd);
		}
	}

	// eliberarea memoriei alocate
	free(action);
	free(cookie);
	free(token);

	return 0;
}
