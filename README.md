# Bank Management System

This is a simple command-line-based bank management system implemented in C++. It allows users to create accounts, manage personal information, deposit and withdraw money, transfer funds, and view transaction history.

## Table of Contents

- [Technologies](#technologies)
- [Features](#features)
  - [Main Menu](#main-menu)
  - [User Menu](#user-menu)
- [Usage](#usage)
- [Data Storage](#data-storage)
- [Contributing](#contributing)
- [License](#license)

## Technologies

- **C++**: The entire project is written in C++, making use of its features like object-oriented programming (OOP) and standard template library (STL).

- **Object-Oriented Programming (OOP)**: The code follows an object-oriented design approach with classes and objects for better code organization and modularity.

- **File I/O (File Streams)**: File streams are used to read and write data to text files, enabling data persistence for user and account information.

- **Standard Template Library (STL)**: The STL is used extensively for containers like vectors and maps, making it easier to manage and manipulate data structures.

- **Input Validation**: The code includes input validation to ensure data integrity and to handle user input errors gracefully.

## Features

### Main Menu

- **Login**: Existing users can log in to their accounts.
- **Sign Up**: New users can create accounts.

### User Menu

Once logged in, users have access to the following options:

- **Account Information**: View account details and balance.
- **Personal Information**: View personal information.
- **Edit Personal Information**: Change personal details like first name, last name, email, username, and password.
- **Transaction History**: View a list of all transactions made on the account.
- **Transfer Money**: Transfer funds to another user's account.
- **Deposit Money**: Deposit money into the account.
- **Withdraw Money**: Withdraw money from the account.
- **Log Out**: Log out of the current account.

## Usage

- When prompted to enter a number, you can use the number keys on your keyboard to select options.
- Passwords must be at least 8 characters long, containing numbers, characters, special characters, and at least one uppercase letter.

## Data Storage

User and account data is stored in plain text files:

- `users.txt`: Contains user information.
- `accounts.txt`: Contains account information.
- `history.txt`: Contains transaction history.

## Contributing

Feel free to contribute to this project by submitting issues or pull requests.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

Enjoy using the Bank Management System! If you encounter any issues or have suggestions for improvement, please don't hesitate to let us know.
