#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define LIMIT 30000
#define MAX_CLIENTS 200
#define PORT 8080

// Data structures
typedef enum {
	Cultural,
	Adventural,
	Beach
} Category;

struct Details {
	char name[100];
	int age;
	char gender[10];
	int index;
};

struct TourismPlace {
	char departureAddress[30];
	char arrivalAddress[30];
	float price;
	Category category;
	int seatsAvailable;
	char date[15];
	bool isRoundTrip;
};

struct filterData {
	int size;
	char browse[LIMIT];
};

int ticketID;
struct TourismPlace places[] = {
        { "Mahabalipuram", "Mumbai", 13969.52, Beach, 33, "2023-10-03", true },
        { "Kanyakumari", "Delhi", 17667.28, Beach, 16, "2023-01-04", true },
        { "Goa", "Delhi", 5554.42, Adventural, 11, "2023-08-26", false },
        { "Madurai", "Ahmedabad", 4765.62, Cultural, 35, "2023-07-22", false },
        { "Rishikesh", "Delhi", 19596.02, Adventural, 2, "2023-04-20", true },
        { "Khajuraho", "Ahmedabad", 14251.94, Cultural, 14, "2023-06-20", true },
        { "Kanyakumari", "Lucknow", 3893.37, Beach, 44, "2023-01-05", false },
        { "Kolkata", "Pune", 8192.76, Cultural, 43, "2023-10-11", false },
        { "Corbett", "Nagpur", 19116.24, Adventural, 49, "2023-01-14", true },
        { "Kolkata", "Chennai", 2244.45, Cultural, 31, "2023-10-07", false },
        { "Kochi", "Hyderabad", 7303.46, Cultural, 48, "2023-03-21", true },
        { "Kovalam", "Kolkata", 8741.12, Beach, 28, "2023-06-05", false },
        { "Mysore", "Nagpur", 8579.24, Cultural, 34, "2023-07-05", true },
        { "Kochi", "Hyderabad", 3651.73, Cultural, 48, "2023-03-21", false },
        { "Andaman", "Ahmedabad", 18362.58, Beach, 46, "2023-07-09", true },
        { "Rishikesh", "Delhi", 7772.78, Adventural, 21, "2023-11-14", true },
        { "Kolkata", "Mumbai", 9862.14, Cultural, 4, "2023-09-05", true },
        { "Pondicherry", "Delhi", 2753.00, Beach, 39, "2023-04-16", false },
        { "Rishikesh", "Delhi", 9798.01, Adventural, 2, "2023-04-20", false },
        { "Madurai", "Nagpur", 9452.13, Cultural, 31, "2023-06-23", false },
        { "Udaipur", "Ahmedabad", 8101.68, Cultural, 25, "2023-06-16", true },
        { "Agra", "Ahmedabad", 18237.52, Cultural, 44, "2023-07-22", true },
        { "Varkala", "Hyderabad", 13823.56, Beach, 26, "2023-01-18", true },
        { "Mahabalipuram", "Hyderabad", 5423.06, Beach, 11, "2023-06-04", true },
        { "Tarkarli", "Delhi", 6316.11, Beach, 1, "2023-04-28", false },
        { "Alleppey", "Delhi", 8048.94, Beach, 42, "2023-05-23", false },
        { "Auli", "Hyderabad", 10830.68, Adventural, 7, "2023-04-23", true },
        { "Rishikesh", "Chennai", 4168.42, Adventural, 1, "2023-09-09", false },
        { "Agra", "Pune", 9175.02, Cultural, 5, "2023-08-08", true },
        { "Rishikesh", "Kolkata", 9104.47, Adventural, 42, "2023-10-15", false },
        { "Udaipur", "Pune", 3507.88, Cultural, 18, "2023-08-22", false },
        { "Madurai", "Ahmedabad", 5463.55, Cultural, 25, "2023-09-28", false },
        { "Varanasi", "Hyderabad", 8830.61, Cultural, 44, "2023-06-16", false },
        { "Goa", "Kolkata", 14013.68, Beach, 4, "2023-04-01", true },
        { "Varkala", "Mumbai", 8452.81, Beach, 9, "2023-01-04", false },
        { "Goa", "Mumbai", 11165.48, Adventural, 14, "2023-06-05", true },
        { "Auli", "Bangalore", 4333.12, Adventural, 4, "2023-12-21", false },
        { "Manali", "Chennai", 9523.22, Adventural, 22, "2023-01-25", true },
        { "Varkala", "Hyderabad", 6911.78, Beach, 26, "2023-01-18", false },
        { "Andaman", "Nagpur", 13563.66, Beach, 11, "2023-02-18", true },
        { "Kolkata", "Pune", 18900.60, Cultural, 1, "2023-05-08", true },
        { "Goa", "Kolkata", 7006.84, Beach, 4, "2023-04-01", false },
        { "Andaman", "Chennai", 11460.14, Adventural, 27, "2023-09-22", true },
        { "Madurai", "Ahmedabad", 10927.10, Cultural, 25, "2023-09-28", true },
        { "Rishikesh", "Mumbai", 7051.91, Adventural, 33, "2023-08-08", false },
        { "Andaman", "Ahmedabad", 9181.29, Beach, 46, "2023-07-09", false },
        { "Kanyakumari", "Delhi", 8833.64, Beach, 16, "2023-01-04", false },
        { "Goa", "Chennai", 5174.26, Beach, 15, "2023-09-16", true },
        { "Tarkarli", "Delhi", 8120.92, Beach, 23, "2023-07-23", false },
        { "Jaipur", "Pune", 7437.85, Cultural, 24, "2023-02-21", false },
        { "Alleppey", "Hyderabad", 10157.88, Beach, 22, "2023-12-19", true },
        { "Haridwar", "Bangalore", 12976.94, Cultural, 17, "2023-01-13", true },
        { "Goa", "Nagpur", 8379.68, Adventural, 34, "2023-04-24", true },
        { "Mysore", "Kolkata", 7292.98, Cultural, 42, "2023-04-18", false },
        { "Madurai", "Bangalore", 15751.74, Cultural, 29, "2023-11-06", true },
        { "Goa", "Bangalore", 6628.56, Beach, 39, "2023-08-28", true },
        { "Pondicherry", "Chennai", 8372.83, Beach, 16, "2023-10-19", false },
        { "Kovalam", "Nagpur", 6671.17, Beach, 24, "2023-01-05", false },
        { "Udaipur", "Ahmedabad", 4050.84, Cultural, 25, "2023-06-16", false },
        { "Mysore", "Pune", 9848.08, Cultural, 36, "2023-10-06", false },
        { "Tarkarli", "Delhi", 13134.14, Beach, 11, "2023-02-05", true },
        { "Goa", "Nagpur", 4189.84, Adventural, 34, "2023-04-24", false },
        { "Agra", "Nagpur", 7169.55, Cultural, 5, "2023-02-03", false },
        { "Pondicherry", "Nagpur", 9753.06, Beach, 29, "2023-08-02", false },
        { "Mahabalipuram", "Ahmedabad", 18102.62, Beach, 18, "2023-07-28", true },
        { "Rishikesh", "Delhi", 3886.39, Adventural, 21, "2023-11-14", false },
        { "Andaman", "Ahmedabad", 11091.30, Adventural, 47, "2023-07-16", true },
        { "Mysore", "Kolkata", 14585.96, Cultural, 42, "2023-04-18", true },
        { "Kolkata", "Chennai", 3143.65, Cultural, 1, "2023-07-16", false },
        { "Pondicherry", "Nagpur", 19506.12, Beach, 29, "2023-08-02", true },
        { "Kovalam", "Delhi", 5641.40, Beach, 20, "2023-05-15", true },
        { "Haridwar", "Bangalore", 6488.47, Cultural, 17, "2023-01-13", false },
        { "Goa", "Lucknow", 2746.81, Beach, 18, "2023-01-14", false },
        { "Manali", "Chennai", 2256.46, Adventural, 49, "2023-01-25", false },
        { "Kolkata", "Hyderabad", 18651.90, Cultural, 33, "2023-10-03", true },
        { "Tarkarli", "Delhi", 16241.84, Beach, 23, "2023-07-23", true },
        { "Pondicherry", "Delhi", 5506.00, Beach, 39, "2023-04-16", true },
        { "Mahabalipuram", "Lucknow", 17683.32, Beach, 37, "2023-03-14", true },
        { "Pondicherry", "Pune", 9398.60, Beach, 9, "2023-04-24", false },
        { "Goa", "Chennai", 2587.13, Beach, 15, "2023-09-16", false },
        { "Gokarna", "Chennai", 9402.80, Beach, 45, "2023-03-13", false },
        { "Tarkarli", "Pune", 5172.44, Beach, 26, "2023-07-17", true },
        { "Kochi", "Hyderabad", 9592.27, Cultural, 43, "2023-06-01", false },
        { "Udaipur", "Pune", 7015.76, Cultural, 18, "2023-08-22", true },
        { "Kolkata", "Hyderabad", 9325.95, Cultural, 33, "2023-10-03", false },
        { "Kovalam", "Nagpur", 13342.34, Beach, 24, "2023-01-05", true },
        { "Agra", "Nagpur", 14339.10, Cultural, 5, "2023-02-03", true },
        { "Goa", "Pune", 7391.92, Beach, 35, "2023-08-12", false },
        { "Kolkata", "Chennai", 4488.90, Cultural, 31, "2023-10-07", true },
        { "Khajuraho", "Hyderabad", 8510.02, Cultural, 33, "2023-03-06", true },
        { "Corbett", "Nagpur", 9558.12, Adventural, 49, "2023-01-14", false },
        { "Mahabalipuram", "Mumbai", 6984.76, Beach, 33, "2023-10-03", false },
        { "Tarkarli", "Delhi", 12632.22, Beach, 1, "2023-04-28", true },
        { "Andaman", "Mumbai", 12740.74, Beach, 24, "2023-02-12", true },
        { "Goa", "Lucknow", 5493.62, Beach, 18, "2023-01-14", true },
        { "Alleppey", "Ahmedabad", 2234.50, Beach, 32, "2023-02-22", false },
        { "Kochi", "Hyderabad", 19184.54, Cultural, 43, "2023-06-01", true },
        { "Manali", "Delhi", 19320.10, Adventural, 47, "2023-03-09", true },
        { "Kanyakumari", "Pune", 16229.14, Beach, 9, "2023-07-03", true },
        { "Gokarna", "Chennai", 18805.60, Beach, 45, "2023-03-13", true },
        { "Kochi", "Bangalore", 4373.38, Cultural, 13, "2023-06-15", true },
        { "Mysore", "Pune", 19696.16, Cultural, 36, "2023-10-06", true },
        { "Varanasi", "Ahmedabad", 8893.64, Cultural, 47, "2023-10-19", true },
        { "Andaman", "Chennai", 9049.84, Adventural, 32, "2023-05-28", true },
        { "Varanasi", "Hyderabad", 13563.78, Cultural, 35, "2023-05-14", true },
        { "Mahabalipuram", "Ahmedabad", 7912.58, Beach, 6, "2023-09-05", true },
        { "Manali", "Chennai", 4512.92, Adventural, 49, "2023-01-25", true },
        { "Madurai", "Nagpur", 18904.26, Cultural, 31, "2023-06-23", true },
        { "Tarkarli", "Delhi", 6567.07, Beach, 11, "2023-02-05", false },
        { "Manali", "Lucknow", 14257.76, Adventural, 36, "2023-12-11", true },
        { "Udaipur", "Chennai", 8050.56, Cultural, 24, "2023-02-28", false },
        { "Varkala", "Nagpur", 13276.56, Beach, 25, "2023-02-27", true },
        { "Kolkata", "Mumbai", 4931.07, Cultural, 4, "2023-09-05", false },
        { "Kolkata", "Pune", 16385.52, Cultural, 43, "2023-10-11", true },
        { "Kolkata", "Chennai", 6287.30, Cultural, 1, "2023-07-16", true },
        { "Gokarna", "Nagpur", 3803.12, Beach, 49, "2023-10-20", false },
        { "Corbett", "Ahmedabad", 18321.56, Adventural, 45, "2023-01-03", true },
        { "Madurai", "Bangalore", 7875.87, Cultural, 29, "2023-11-06", false },
        { "Andaman", "Ahmedabad", 5545.65, Adventural, 47, "2023-07-16", false },
        { "Gokarna", "Nagpur", 7606.24, Beach, 49, "2023-10-20", true },
        { "Khajuraho", "Hyderabad", 4255.01, Cultural, 33, "2023-03-06", false },
        { "Pondicherry", "Pune", 18797.20, Beach, 9, "2023-04-24", true },
        { "Agra", "Ahmedabad", 9118.76, Cultural, 44, "2023-07-22", false },
        { "Manali", "Mumbai", 6690.22, Adventural, 46, "2023-08-08", true },
        { "Madurai", "Ahmedabad", 9531.24, Cultural, 35, "2023-07-22", true },
        { "Goa", "Mumbai", 6480.19, Adventural, 42, "2023-04-18", false },
        { "Haridwar", "Bangalore", 7625.40, Cultural, 26, "2023-12-26", false },
        { "Alleppey", "Ahmedabad", 8307.00, Beach, 46, "2023-01-25", true },
        { "Goa", "Delhi", 11108.84, Adventural, 11, "2023-08-26", true },
        { "Tarkarli", "Pune", 2586.22, Beach, 26, "2023-07-17", false },
        { "Bir Billing", "Hyderabad", 15875.48, Adventural, 5, "2023-05-11", true },
        { "Auli", "Chennai", 16650.02, Adventural, 29, "2023-09-27", true },
        { "Kanyakumari", "Lucknow", 7786.74, Beach, 44, "2023-01-05", true },
        { "Udaipur", "Chennai", 16101.12, Cultural, 24, "2023-02-28", true },
        { "Alleppey", "Ahmedabad", 4153.50, Beach, 46, "2023-01-25", false },
        { "Bir Billing", "Hyderabad", 7937.74, Adventural, 5, "2023-05-11", false },
        { "Kanyakumari", "Bangalore", 3653.01, Beach, 10, "2023-06-12", false },
        { "Kovalam", "Delhi", 2820.70, Beach, 20, "2023-05-15", false },
        { "Pondicherry", "Ahmedabad", 8437.59, Beach, 24, "2023-01-27", false },
        { "Kochi", "Nagpur", 12860.12, Cultural, 38, "2023-06-16", true },
        { "Kochi", "Nagpur", 6430.06, Cultural, 38, "2023-06-16", false },
        { "Andaman", "Mumbai", 6370.37, Beach, 24, "2023-02-12", false },
        { "Varkala", "Nagpur", 6638.28, Beach, 25, "2023-02-27", false },
        { "Khajuraho", "Ahmedabad", 7125.97, Cultural, 14, "2023-06-20", false },
        { "Manali", "Chennai", 4761.61, Adventural, 22, "2023-01-25", false },
        { "Tarkarli", "Lucknow", 5873.50, Beach, 3, "2023-03-22", true },
        { "Andaman", "Chennai", 5730.07, Adventural, 27, "2023-09-22", false },
        { "Madurai", "Delhi", 14851.60, Cultural, 7, "2023-02-07", true },
        { "Kochi", "Bangalore", 17810.50, Cultural, 26, "2023-11-02", true },
        { "Agra", "Mumbai", 7795.24, Cultural, 48, "2023-08-21", true },
        { "Mysore", "Chennai", 8914.09, Cultural, 34, "2023-06-20", false },
        { "Varanasi", "Hyderabad", 17661.22, Cultural, 44, "2023-06-16", true },
        { "Agra", "Mumbai", 3897.62, Cultural, 48, "2023-08-21", false },
        { "Rishikesh", "Mumbai", 14103.82, Adventural, 33, "2023-08-08", true },
        { "Goa", "Mumbai", 12960.38, Adventural, 42, "2023-04-18", true },
        { "Mahabalipuram", "Ahmedabad", 3956.29, Beach, 6, "2023-09-05", false },
        { "Kanyakumari", "Bangalore", 7306.02, Beach, 10, "2023-06-12", true },
        { "Haridwar", "Bangalore", 15250.80, Cultural, 26, "2023-12-26", true },
        { "Varanasi", "Ahmedabad", 4446.82, Cultural, 47, "2023-10-19", false },
        { "Pondicherry", "Ahmedabad", 16875.18, Beach, 24, "2023-01-27", true },
        { "Auli", "Chennai", 8325.01, Adventural, 29, "2023-09-27", false },
        { "Mahabalipuram", "Ahmedabad", 9051.31, Beach, 18, "2023-07-28", false },
        { "Goa", "Mumbai", 5582.74, Adventural, 14, "2023-06-05", false },
        { "Agra", "Pune", 4587.51, Cultural, 5, "2023-08-08", false },
        { "Kolkata", "Pune", 9450.30, Cultural, 1, "2023-05-08", false },
        { "Kanyakumari", "Pune", 8114.57, Beach, 9, "2023-07-03", false },
        { "Auli", "Bangalore", 8666.24, Adventural, 4, "2023-12-21", true },
        { "Mahabalipuram", "Hyderabad", 2711.53, Beach, 11, "2023-06-04", false },
        { "Manali", "Lucknow", 7128.88, Adventural, 36, "2023-12-11", false },
        { "Andaman", "Nagpur", 6781.83, Beach, 11, "2023-02-18", false },
        { "Alleppey", "Hyderabad", 5078.94, Beach, 22, "2023-12-19", false },
        { "Alleppey", "Ahmedabad", 4469.00, Beach, 32, "2023-02-22", true },
        { "Goa", "Bangalore", 3314.28, Beach, 39, "2023-08-28", false },
        { "Kochi", "Delhi", 6120.82, Cultural, 41, "2023-06-24", false },
        { "Auli", "Hyderabad", 5415.34, Adventural, 7, "2023-04-23", false },
        { "Andaman", "Chennai", 4524.92, Adventural, 32, "2023-05-28", false },
        { "Rishikesh", "Chennai", 8336.84, Adventural, 1, "2023-09-09", true },
        { "Kovalam", "Kolkata", 17482.24, Beach, 28, "2023-06-05", true },
        { "Goa", "Chennai", 17276.04, Beach, 25, "2023-07-20", true },
        { "Goa", "Chennai", 8638.02, Beach, 25, "2023-07-20", false },
        { "Mysore", "Chennai", 17828.18, Cultural, 34, "2023-06-20", true },
        { "Alleppey", "Delhi", 16097.88, Beach, 42, "2023-05-23", true },
        { "Manali", "Delhi", 9660.05, Adventural, 47, "2023-03-09", false },
        { "Kochi", "Bangalore", 2186.69, Cultural, 13, "2023-06-15", false },
        { "Varanasi", "Hyderabad", 6781.89, Cultural, 35, "2023-05-14", false },
        { "Jaipur", "Pune", 14875.70, Cultural, 24, "2023-02-21", true },
        { "Mahabalipuram", "Lucknow", 8841.66, Beach, 37, "2023-03-14", false },
        { "Kanyakumari", "Nagpur", 6439.43, Beach, 17, "2023-11-15", false },
        { "Varkala", "Mumbai", 16905.62, Beach, 9, "2023-01-04", true },
        { "Pondicherry", "Chennai", 16745.66, Beach, 16, "2023-10-19", true },
        { "Goa", "Pune", 14783.84, Beach, 35, "2023-08-12", true },
        { "Madurai", "Delhi", 7425.80, Cultural, 7, "2023-02-07", false },
        { "Kochi", "Bangalore", 8905.25, Cultural, 26, "2023-11-02", false },
        { "Tarkarli", "Lucknow", 2936.75, Beach, 3, "2023-03-22", false },
        { "Mysore", "Nagpur", 4289.62, Cultural, 34, "2023-07-05", false },
        { "Corbett", "Ahmedabad", 9160.78, Adventural, 45, "2023-01-03", false },
        { "Kanyakumari", "Nagpur", 12878.86, Beach, 17, "2023-11-15", true },
        { "Kochi", "Delhi", 12241.64, Cultural, 41, "2023-06-24", true },
        { "Manali", "Mumbai", 3345.11, Adventural, 46, "2023-08-08", false },
        { "Rishikesh", "Kolkata", 18208.94, Adventural, 42, "2023-10-15", true }
};

// Function Prototypes
char *categoryToString(Category cat);
int filterByPlace(const struct TourismPlace tourismData[], int numPlaces, const char* destinationAddress, char* result);
int filterByCategory(const struct TourismPlace tourismData[], int numPlaces, Category category, char* result);
void placesToTable(const struct TourismPlace tourismData[], int numPlaces, char* result);
void getDestination(char *result);
int sendData(int descriptor, char *data);
void implement_tour_app(int descriptor);
void *handle_client(void *socket_desc);
int communicate(int client_socket);
void constructHTML(char *htmlContent, int index, struct Details data);
char* floatToString(float num);
char* intToString(int num);

// Data types
int filterPlace[200];
int filterCategory[200];
const char* random_cities[] = {
	"Mumbai", "Delhi", "Bangalore", "Chennai", "Hyderabad",
	"Kolkata", "Ahmedabad", "Lucknow", "Pune", "Nagpur"
};

const char* cultural_destinations[] = {
	"Jaipur", "Agra", "Varanasi", "Khajuraho", "Kolkata",
	"Mysore", "Udaipur", "Madurai", "Kochi", "Haridwar"
};

const char* adventure_destinations[] = {
	"Manali", "Rishikesh", "Leh", "Goa", "Auli",
	"Andaman", "Bir Billing", "Spiti Valley", "Dandeli", "Corbett"
};

const char* beach_destinations[] = {
	"Goa", "Kovalam", "Pondicherry", "Varkala", "Mahabalipuram",
	"Gokarna", "Andaman", "Tarkarli", "Kanyakumari", "Alleppey"
};

const char* all_destinations[] = {
	"Jaipur", "Agra", "Varanasi", "Khajuraho", "Kolkata",
	"Mysore", "Udaipur", "Madurai", "Kochi", "Haridwar",
	"Manali", "Rishikesh", "Leh", "Goa", "Auli",
	"Andaman", "Bir Billing", "Spiti Valley", "Dandeli", "Corbett",
	"Goa", "Kovalam", "Pondicherry", "Varkala", "Mahabalipuram",
	"Gokarna", "Andaman", "Tarkarli", "Kanyakumari", "Alleppey"
};

int main() {

	ticketID = rand();
        int server_socket, client_socket;
        struct sockaddr_in server_addr, client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        pthread_t thread_id;

        // Create socket
        printf("Creating socket...\n");
        server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket == -1) {
                perror("  Socket creation failed");
                exit(1);
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(PORT);
        server_addr.sin_addr.s_addr = INADDR_ANY;

        // Bind
        printf("Binding...\n");
        if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
                perror("  Binding failed");
                exit(1);
        }

        // Listen
        printf("Listening...\n");
        if (listen(server_socket, MAX_CLIENTS) < 0) {
                perror("  Listening failed");
                exit(1);
        }

        printf("Server is listening on port %d\n", PORT);

        while (1) {
                // Accept a connection
                client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
                if (client_socket < 0) {
                        perror("  Accepting client connection failed");
                        exit(1);
                }

                printf("Reading descriptor: %d\n", client_socket);

                // Create a thread for the client
                if (pthread_create(&thread_id, NULL, handle_client, (void *)&client_socket) < 0) {
                        perror("  Thread creation failed");
                        exit(1);
                }
        }

        close(server_socket);
        return 0;
}

void *handle_client(void *socket_desc) {
        int client_socket = *(int *)socket_desc;

        // Handle client communication
        implement_tour_app(client_socket);

        close(client_socket);

        return NULL;
}

int sendData(int descriptor, char *data) {
	printf("  Sending message to descriptor: %d\n", descriptor);
	
	int rc = send(descriptor, data, strlen(data), 0);
	if (rc < 0) {
		perror("  send() failed");
		return 1;
	}
	
	printf("%d bytes data sent successfully to descriptor %d\n", rc, descriptor);
	return 0;
}

void implement_tour_app(int descriptor) {
	
	char browsing[LIMIT];
	char instructions[] = 
	    "========== Travel Planner ==========\n"
	    "1. Browse Destinations\n"
	    "2. Filter Trips by Place\n"
	    "3. Filter Trips by Category\n"
	    "4. Exit\n"
	    "=====================================\n"
	    "Enter your choice: ";
	    
    MAIN_MENU:
	if (sendData(descriptor, instructions)) return;
	
	int input;
	if (recv(descriptor, &input, sizeof(input), 0) < 0) {
		perror(" recv() failed");
		return;
	}
	
	switch (input) {
		case 1: {
			bzero(browsing, strlen(browsing));
			placesToTable(places, 200, browsing);
			char newInstruction[] = 
			    "\n========= Travel Planner =========\n"
			    "1. Filter by Place\n"
			    "2. Filter by Category\n"
			    "3. Choose your option\n"
			    "4. Go Back to Main Menu\n"
			    "5. Exit\n"
			    "====================================\n"
			    "Enter your choice: ";
			
			strcat(browsing, newInstruction);
			if (sendData(descriptor, browsing)) return;
			
			int choice;
			if (recv(descriptor, &choice, sizeof(choice), 0) < 0) {
				perror(" recv() failed");
				return;
			}
			
			switch (choice) {
				case 1: goto PLACE; break;
				case 2: goto CATEGORY; break;
				case 3: {
					int len = strlen(browsing);
					int instLen = strlen(newInstruction);
					
					memset(browsing + len - instLen, '\0', instLen);
					strcat(browsing, "\nEnter your choice: ");
					if (sendData(descriptor, browsing)) return;
					
					struct Details data;
					
					if (recv(descriptor, &data, sizeof(data), 0) < 0) {
						perror(" recv() failed");
						return;
					}
					
					bzero(browsing, strlen(browsing));
					constructHTML(browsing, data.index-1, data);

					browsing[strlen(browsing)] = '\0';
					if (sendData(descriptor, browsing)) return;
					
				} break;
				case 4: goto MAIN_MENU; break;
			}
				
		} break;
		
	    PLACE:
		case 2: {
			bzero(browsing, sizeof(browsing));
			getDestination(browsing);
			char newInstruction[] = "\nEnter your choice: ";
			strcat(browsing, newInstruction);
			
			if (sendData(descriptor, browsing)) return;
			
			int choice;
			if (recv(descriptor, &choice, sizeof(choice), 0) < 0) {
				perror(" recv() failed");
				return;
			}
			
			bzero(browsing, sizeof(browsing));
			
			int size = filterByPlace(places, 200, all_destinations[choice-1], browsing);
			strcat(browsing, newInstruction);
			
			struct filterData data;
			data.size = size;
			strcpy(data.browse, browsing);
			printf("  Sending message to descriptor: %d\n", descriptor);
			if (send(descriptor, &data, sizeof(data), 0) < 0) {
				perror("  send() failed");
				return;
			}
			
			struct Details details;
			
			if (recv(descriptor, &details, sizeof(details), 0) < 0) {
				perror(" recv() failed");
				return;
			}

			bzero(browsing, sizeof(browsing));
			constructHTML(browsing, filterPlace[details.index], details);

			// printf("%s\n", browsing);
			browsing[strlen(browsing)] = '\0';
			if (sendData(descriptor, browsing)) return;
			
		} break;
	    CATEGORY:
		case 3: {
			char newInstruction[] = 
			    "\n========= Travel Planner =========\n"
			    "1. Cultural\n"
			    "2. Adventural\n"
			    "3. Beach\n"
			    "====================================\n"
			    "Enter your choice: ";
			    
			if (sendData(descriptor, newInstruction)) return;
			
			int choice;
			if (recv(descriptor, &choice, sizeof(choice), 0) < 0) {
				perror(" recv() failed");
				return;
			}
			
			Category categoryChoice[] = {Cultural, Adventural, Beach};
			
			bzero(browsing, sizeof(browsing));
			int size = filterByCategory(places, 200, categoryChoice[choice-1], browsing);
			strcat(browsing, "\nEnter your choice: ");
			
			struct filterData data;
			data.size = size;
			strcpy(data.browse, browsing);
			printf("  Sending message to descriptor: %d\n", descriptor);
			if (send(descriptor, &data, sizeof(data), 0) < 0) {
				perror("  send() failed");
				return;
			}
			
			struct Details details;
			if (recv(descriptor, &details, sizeof(details), 0) < 0) {
				perror(" recv() failed");
				return;
			}

			bzero(browsing, sizeof(browsing));
			constructHTML(browsing, filterCategory[details.index], details);

			//printf("%s\n", strlen(browsing), browsing);
			if (sendData(descriptor, browsing)) return;
		} break;
	}
}

char *categoryToString(Category cat) {
	switch (cat) {
		case Cultural: return "Cultural";
		case Adventural: return "Adventural";
		case Beach: return "Beach";
	}
}

int filterByPlace(const struct TourismPlace tourismData[], int numPlaces, const char* destinationAddress, char* result) {
    	sprintf(result, "|%-4s|%-30s|%-30s|%-12s|%-10s|%-15s|%-12s|%-12s|\n", "Sno", "Departure Address", "Arrival Address", 
    			"Price", "Category","Seats Available", "Date", "Round Trip");
    	int rowCounter = 1;
    	char row[1024];

    	for (int i = 0; i < numPlaces; i++) {
        	if (strcmp(tourismData[i].departureAddress, destinationAddress) == 0) {
            		sprintf(row, "|%-4d|%-30s|%-30s|%-12.2f|%-10s|%-15d|%-12s|%-12s|\n", rowCounter, tourismData[i].departureAddress,
                    		tourismData[i].arrivalAddress, tourismData[i].price, categoryToString(tourismData[i].category),
                    		tourismData[i].seatsAvailable, tourismData[i].date, tourismData[i].isRoundTrip ? "Yes" : "No");
            		strcat(result, row);
            		bzero(row, sizeof(row));

            		// Store the index of tourismData in the filterPlace array
            		filterPlace[rowCounter++] = i;
        	}
    	}
    	
    	return rowCounter;
}

int filterByCategory(const struct TourismPlace tourismData[], int numPlaces, Category category, char* result) {
    	sprintf(result, "|%-4s|%-30s|%-30s|%-12s|%-10s|%-15s|%-12s|%-12s|\n", "Sno", "Departure Address", "Arrival Address", 
    			"Price", "Category","Seats Available", "Date", "Round Trip");
    	int rowCounter = 1;
        char row[1024];
        
        

    	for (int i = 0; i < numPlaces; i++) {
        	if (tourismData[i].category == category) {
            		sprintf(row, "|%-4d|%-30s|%-30s|%-12.2f|%-10s|%-15d|%-12s|%-12s|\n", rowCounter, tourismData[i].departureAddress,
                    		tourismData[i].arrivalAddress, tourismData[i].price, categoryToString(tourismData[i].category),
                    		tourismData[i].seatsAvailable, tourismData[i].date, tourismData[i].isRoundTrip ? "Yes" : "No");
            		strcat(result, row);
            		bzero(row, sizeof(row));

		    	// Store the index of tourismData in the filterCategory array
		    	filterCategory[rowCounter++] = i;
        	}
    	}
    	
    	return rowCounter;
}

void placesToTable(const struct TourismPlace tourismData[], int numPlaces, char *result) {

    	sprintf(result, "|%-4s|%-30s|%-30s|%-12s|%-10s|%-15s|%-12s|%-12s|\n", "Sno", "Departure Address", "Arrival Address", 
    			"Price", "Category","Seats Available", "Date", "Round Trip");
	
    	char row[1024];
    	for (int i = 0; i < numPlaces; i++) {
        	sprintf(row, "|%-4d|%-30s|%-30s|%-12.2f|%-10s|%-15d|%-12s|%-12s|\n", i + 1, tourismData[i].departureAddress,
        	tourismData[i].arrivalAddress, tourismData[i].price, categoryToString(tourismData[i].category), tourismData[i].seatsAvailable,
        	tourismData[i].date, tourismData[i].isRoundTrip ? "Yes" : "No");
        	
        	strcat(result, row);
    		bzero(row, sizeof(row));
    	}
}

char* intToString(int num) {
    char* str = (char*)malloc(10); // Allocate memory for the string

    if (str != NULL) {
        sprintf(str, "%d", num);
    }

    return str;
}

char* floatToString(float num) {
    char format[20];
    sprintf(format, "%.2f", num);

    char* str = (char*)malloc(strlen(format) + 1); // Allocate memory for the string

    if (str != NULL) {
        strcpy(str, format);
    }

    return str;
}

void getDestination(char *result) {

	char numbering[6];
	strcat(result, "Cultural\n");
	for(int i = 0; i < 10; i++) {
		sprintf(numbering, "\t%d: ", i+1);
		strcat(result, numbering);
		bzero(numbering, sizeof(numbering));
		strcat(result, cultural_destinations[i]);
		strcat(result, "\n");
	}
	
	strcat(result, "\nAdventural\n");
	
	for(int i = 0; i < 10; i++) {
		sprintf(numbering, "\t%d: ", i+11);
		strcat(result, numbering);
		bzero(numbering, sizeof(numbering));
		strcat(result, adventure_destinations[i]);
		strcat(result, "\n");
	}
	
	strcat(result, "\nBeach\n");
	
	for(int i = 0; i < 10; i++) {
		sprintf(numbering, "\t%d: ", i+21);
		strcat(result, numbering);
		bzero(numbering, sizeof(numbering));
		strcat(result, beach_destinations[i]);
		strcat(result, "\n");
	}
	
	strcat(result, "\n");
}

void constructHTML(char *htmlContent, int index, struct Details data) {
	char seatConfirmation[20];
	snprintf(seatConfirmation, sizeof(seatConfirmation), places[index].seatsAvailable-- == 0 ? "Waiting List" : "Confirmed");
	char trip[20];
	snprintf(trip, sizeof(trip), places[index].isRoundTrip == true ? "Round Trip Tour" : "One Way Tour");

	strcat(htmlContent, "<!DOCTYPE html>\n");
	strcat(htmlContent, "<html lang=\"en\">\n");
	strcat(htmlContent, "<head>\n");
	strcat(htmlContent, "    <meta charset=\"UTF-8\">\n");
	strcat(htmlContent, "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
	strcat(htmlContent, "    <title>Tour Ticket</title>\n");
	strcat(htmlContent, "    <style>\n");
	strcat(htmlContent, "        body {\n");
	strcat(htmlContent, "            font-family: Arial, sans-serif;\n");
	strcat(htmlContent, "            text-align: center;\n");
	strcat(htmlContent, "        }\n");
	strcat(htmlContent, "        .ticket {\n");
	strcat(htmlContent, "            border: 2px solid #000;\n");
	strcat(htmlContent, "            width: 500px;\n");
	strcat(htmlContent, "            height: 340px;\n");
	strcat(htmlContent, "            margin: 20px auto;\n");
	strcat(htmlContent, "            padding: 10px;\n");
	strcat(htmlContent, "            background-color: #f3f3f3;\n");
	strcat(htmlContent, "        }\n");
	strcat(htmlContent, "        .ticket h2 {\n");
	strcat(htmlContent, "            font-size: 1.5em;\n");
	strcat(htmlContent, "            margin: 0;\n");
	strcat(htmlContent, "        }\n");
	strcat(htmlContent, "        .ticket p {\n");
	strcat(htmlContent, "            font-size: 1.2em;\n");
	strcat(htmlContent, "            margin: 5px 0;\n");
	strcat(htmlContent, "        }\n");
	strcat(htmlContent, "    </style>\n");
	strcat(htmlContent, "</head>\n");
	strcat(htmlContent, "<body>\n");
	strcat(htmlContent, "    <div class=\"ticket\">\n");
	strcat(htmlContent, "        <h2>Tour Ticket</h2>\n");
	strcat(htmlContent, "        <p><strong>Name:</strong> ");
	strcat(htmlContent, data.name);
	strcat(htmlContent, "</p>\n");
	strcat(htmlContent, "        <p><strong>Age:</strong> ");
	strcat(htmlContent, intToString(data.age));
	strcat(htmlContent, "</p>\n");
	strcat(htmlContent, "        <p><strong>Gender:</strong> ");
	strcat(htmlContent, data.gender);
	strcat(htmlContent, "</p>\n");
	strcat(htmlContent, "        <p><strong>Departure Address:</strong> ");
	strcat(htmlContent, places[index].departureAddress);
	strcat(htmlContent, "</p>\n");
	strcat(htmlContent, "        <p><strong>Arrival Address:</strong> ");
	strcat(htmlContent, places[index].arrivalAddress);
	strcat(htmlContent, "</p>\n");
	strcat(htmlContent, "        <p><strong>Price:</strong> ");
	strcat(htmlContent, floatToString(places[index].price));
	strcat(htmlContent, "</p>\n");
	strcat(htmlContent, "        <p><strong>Seat Confirmation:</strong> ");
	strcat(htmlContent, seatConfirmation);
	strcat(htmlContent, "</p>\n");
	strcat(htmlContent, "        <p><strong>Tour Date:</strong> ");
	strcat(htmlContent, places[index].date);
	strcat(htmlContent, "</p>\n");
	strcat(htmlContent, "        <p><strong>Category:</strong> ");
	strcat(htmlContent, categoryToString(places[index].category));
	strcat(htmlContent, "</p>\n");
	strcat(htmlContent, "        <p><strong>Trip Type:</strong> ");
	strcat(htmlContent, trip);
	strcat(htmlContent, "</p>\n");
	strcat(htmlContent, "        <p><strong>Ticket ID:</strong> ");
	strcat(htmlContent, intToString(ticketID--));
	strcat(htmlContent, "</p>\n");
	strcat(htmlContent, "    </div>\n");
	strcat(htmlContent, "</body>\n");
	strcat(htmlContent, "</html>");
}
