/*
 * Filename: sql_queries.h
 * Created on: June  8, 2024
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 */

#ifndef SQL_QUERIES_H_
#define SQL_QUERIES_H_

#include <string>

/**
 * @brief Namespace that contains the queries to be executed in the database
 **/
namespace query
{
    const std::string CREATE_TABLE_WALLET = "CREATE TABLE IF NOT EXISTS Wallet ("
                                            "wallet_id INT AUTO_INCREMENT PRIMARY KEY,"
                                            "name CHAR(50) NOT NULL,"
                                            "balance REAL NOT NULL"
                                            ");";

    const std::string CREATE_TABLE_CATEGORY =
        "CREATE TABLE IF NOT EXISTS Category ("
        "category_id INT AUTO_INCREMENT PRIMARY KEY,"
        "name CHAR(50) NOT NULL"
        ");";

    const std::string CREATE_TABLE_WALLET_TRANSACTION =
        "CREATE TABLE IF NOT EXISTS WalletTransaction ("
        "wallet_transaction_id INT AUTO_INCREMENT PRIMARY KEY,"
        "wallet_id INT NOT NULL,"
        "category_id INT NOT NULL,"
        "type TEXT CHECK(type IN ('INCOME', 'EXPENSE')) NOT NULL,"
        "date DATE NOT NULL,"
        "amount REAL NOT NULL,"
        "description CHAR(50),"
        "FOREIGN KEY (wallet_id) REFERENCES Wallet(wallet_id),"
        "FOREIGN KEY (category_id) REFERENCES Category(category_id)"
        ");";

    const std::string CREATE_TABLE_TRANSFER =
        "CREATE TABLE IF NOT EXISTS Transfer ("
        "transfer_id INT AUTO_INCREMENT PRIMARY KEY,"
        "sender_wallet_id INT NOT NULL,"
        "receiver_wallet_id INT NOT NULL,"
        "date DATE NOT NULL,"
        "amount REAL NOT NULL,"
        "description CHAR(50),"
        "FOREIGN KEY (sender_wallet_id) REFERENCES Wallet(wallet_id),"
        "FOREIGN KEY (receiver_wallet_id) REFERENCES Wallet(wallet_id)"
        ");";

    const std::string CREATE_TABLE_CREDIT_CARD =
        "CREATE TABLE IF NOT EXISTS CreditCard ("
        "number CHAR(20) PRIMARY KEY,"
        "name CHAR(50) NOT NULL,"
        "max_debt REAL NOT NULL,"
        "expiration_date DATE NOT NULL"
        ");";

    const std::string CREATE_TABLE_CREDIT_CARD_DEBT =
        "CREATE TABLE IF NOT EXISTS CreditCardDebt ("
        "debt_id INT AUTO_INCREMENT PRIMARY KEY,"
        "crc_number CHAR(20) NOT NULL,"
        "category_id INT NOT NULL,"
        "date DATE NOT NULL,"
        "total_amount REAL NOT NULL,"
        "remaining_amount REAL NOT NULL,"
        "description CHAR(50),"
        "FOREIGN KEY (crc_number) REFERENCES CreditCard(number)"
        "FOREIGN KEY (category_id) REFERENCES Category(category_id)"
        ");";

    const std::string CREATE_TABLE_CREDIT_CARD_PAYMENT =
        "CREATE TABLE IF NOT EXISTS CreditCardPayment ("
        "payment_id INT AUTO_INCREMENT PRIMARY KEY,"
        "wallet_id INT NOT NULL,"
        "debit_id INT NOT NULL,"
        "date DATE NOT NULL,"
        "amount REAL NOT NULL,"
        "FOREIGN KEY (wallet_id) REFERENCES Wallet(wallet_id),"
        "FOREIGN KEY (debit_id) REFERENCES CreditCardDebt(debt_id)"
        ");";
} // namespace query
#endif // SQL_QUERIES_H_
