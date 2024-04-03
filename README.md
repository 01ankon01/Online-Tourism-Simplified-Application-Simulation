# Online Tourism Simplified Application

This project is a simulation of an online tourism application similar to MakeMyTrip, offering various travel services and functionalities for users to explore and book trips conveniently. It utilizes a robust server-client architecture implemented using C programming language, TCP, and UDP protocols to facilitate communication between the application's components, ensuring efficient data transmission and handling.

## Features

- Users can explore various travel destinations.
- Users can search for flights, hotels, and other accommodations.
- Users can book trips and manage their bookings.
- Secure authentication and encryption mechanisms ensure data privacy and integrity.
- Scalable architecture capable of handling multiple client requests simultaneously.
- Efficient data transmission and handling through TCP and UDP protocols.

## Installation

1. Clone the repository:

    ```
    git clone https://github.com/your_username/online-tourism-application.git
    ```

2. Navigate to the project directory:

    ```
    cd online-tourism-application
    ```

3. Compile the server and client:

    ```
    gcc -o my_program my_program.c -lpthread
    gcc -o client client.c
    ```

## Usage

1. Start the server:

    ```
    ./server
    ```

2. Run the client application:

    ```
    ./client
    ```

3. Follow the instructions provided by the client application to explore and book trips.

## Configuration

- The server can be configured to handle a specific number of clients simultaneously by adjusting appropriate parameters in the `server.cpp` code.
- Encryption and authentication mechanisms can be implemented within the C++ code based on requirements.
- Make sure the server is running and listening for connections on the specified IP address and port (192.168.64.3:8080). If the server is not running or if the IP address or port is incorrect, the client will fail to connect and may display an error message.

## Contributors

- [Ankon Das](https://github.com/01ankon01)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Inspiration: MakeMyTrip
- Technologies: C, TCP, UDP, Socket Programming

## Support

For any inquiries or support, please [open an issue](https://github.com/01ankon01/online-tourism-application/issues) on GitHub.

## Here are some pictures

![Online-Tourism-Simplified-Application-Simulation
](1.png)

