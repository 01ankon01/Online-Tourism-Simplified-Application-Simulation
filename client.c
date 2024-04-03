#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <signal.h>
#define LIMIT 30000

struct filterData {
	int size;
	char browse[LIMIT];
};

struct Details {
	char name[100];
	int age;
	char gender[10];
	int index;
};

void receive(int client_socket) {

	char buffer[LIMIT];
	int rc;
	bzero(buffer, sizeof(buffer));
	
	rc = recv(client_socket, buffer, sizeof(buffer), 0);
	if (rc < 0) {
		perror("Error receiving data");
		exit(EXIT_FAILURE);
	}
	
	system("clear");
	printf("Server response:\n%s", buffer);
}

int getInput(int inputLimit) {
	int input;
	while(scanf(" %d", &input) != 1 || input < 1 || input > inputLimit) {
		while(getchar() != '\n');
		printf("Invalid input, Enter the valid number between 1 - %d: ", inputLimit);
	}
	
	return input;
}

void getDetails(struct Details *data) {
	system("clear");
	printf("Enter your name: ");
	scanf(" %99[^\n]%*c", data->name);
	
	printf("Enter your age: ");
	data->age = getInput(120);
	
	char *genderMessage = 
	    "1. Male\n"
	    "2. Female\n"
	    "Enter your choice: ";
	
	printf("Select your gender\n%s", genderMessage);
	strcpy(data->gender, getInput(2) == 1 ? "Male" : "Female");
	
	printf("Name: %s\nAge: %d\nGender: %s\n", data->name, data->age, data->gender);
}

void generateTicket(int client_socket) {

	int rc;
	char buffer[LIMIT];
	bzero(buffer, sizeof(buffer));
	const char* filename = "ticket.html";
	
	rc = recv(client_socket, buffer, sizeof(buffer), 0);
	if (rc < 0) {
		perror("Error receiving data");
		exit(EXIT_FAILURE);
	}
	
	FILE* file = fopen(filename, "w");
	
	if (file != NULL) {
		fprintf(file, "%s", buffer);
		fclose(file);
		
		printf("Ticket generated successfully!\n");
		char command[50];
		bzero(command, sizeof(command));
    		snprintf(command, sizeof(command), "xdg-open %s", filename);
    		
    		if (system(command) != 0) {
    			perror("Failed to open the web browser");
    			exit(EXIT_FAILURE);
    		}
	} else {
		perror("Failed to open the file");
		exit(EXIT_FAILURE);
	}
}

int main() {

	int client_socket;
    	struct sockaddr_in server_address;

    	// Create a socket
    	client_socket = socket(AF_INET, SOCK_STREAM, 0);
    	if (client_socket == -1) {
		perror("Error creating socket");
		exit(EXIT_FAILURE);
    	}

    	// Configure the server addressFport
    	server_address.sin_family = AF_INET;
    	server_address.sin_port = htons(8080); // Make sure it matches the server's port
    	server_address.sin_addr.s_addr = inet_addr("192.168.64.3"); // Use the server's IP address

    	// Connect to the server
    	if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
		perror("Error connecting to the server");
		exit(EXIT_FAILURE);
    	}
    	
    MAIN_MENU:    	
    	receive(client_socket);
    	int input = getInput(5);
    	
    	if (send(client_socket, &input, sizeof(input), 0) < 0) {
    		perror("Error sending data");
		exit(EXIT_FAILURE);
    	}
    	
    	switch (input) {
		case 1: {
			receive(client_socket);
			int choice = getInput(4);
			
			if (send(client_socket, &choice, sizeof(choice), 0) < 0) {
		    		perror("Error sending data");
				exit(EXIT_FAILURE);
		    	}
			
			switch (choice) {
				case 1: goto PLACE; break;
				case 2: goto CATEGORY; break;
				case 3: {
					receive(client_socket);
					int index = getInput(200);
				    	
				    	struct Details data;				    	
				    	getDetails(&data);
				    	data.index = index;
				    	
				    	if (send(client_socket, &data, sizeof(data), 0) < 0) {
				    		perror("Error sending data");
						exit(EXIT_FAILURE);
				    	}
				    	
				    	generateTicket(client_socket);
				} break;
				case 4: goto MAIN_MENU; break;
				default: printf("Invalid input!\n");
			}
		} break;
	    PLACE:
		case 2: {
			receive(client_socket);
			int choice = getInput(30);
			
			if (send(client_socket, &choice, sizeof(choice), 0) < 0) {
		    		perror("Error sending data");
				exit(EXIT_FAILURE);
		    	}
		    	
		   	struct filterData buffer;
		    	if (recv(client_socket, &buffer, sizeof(buffer), 0) < 0) {
				perror("Error receiving data");
				exit(EXIT_FAILURE);
			}
			
			system("clear");
			printf("Server response:\n%s", buffer.browse);
			
			choice = getInput(buffer.size - 1);
		    	
		    	struct Details data;
		    	
		    	getDetails(&data);
		    	data.index = choice;
		    	
		    	if (send(client_socket, &data, sizeof(data), 0) < 0) {
		    		perror("Error sending data");
				exit(EXIT_FAILURE);
		    	}
		    	
		    	generateTicket(client_socket);
		} break;
	    CATEGORY:
		case 3: {
			receive(client_socket);
			int choice = getInput(3);
			
			if (send(client_socket, &choice, sizeof(choice), 0) < 0) {
		    		perror("Error sending data");
				exit(EXIT_FAILURE);
		    	}
		    	
		   	struct filterData buffer;
		    	if (recv(client_socket, &buffer, sizeof(buffer), 0) < 0) {
				perror("Error receiving data");
				exit(EXIT_FAILURE);
			}
			
			system("clear");
			printf("Server response:\n%s", buffer.browse);
			
			choice = getInput(buffer.size - 1);
		    	struct Details data;
		    	
		    	getDetails(&data);
		    	data.index = choice;
		    	
		    	if (send(client_socket, &data, sizeof(data), 0) < 0) {
		    		perror("Error sending data");
				exit(EXIT_FAILURE);
		    	}
		    	
		    	generateTicket(client_socket);
		} break;		
	}
    
    	// Close the client socket
    	close(client_socket);

    	return 0;
}
