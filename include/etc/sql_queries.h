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
                                            "name CHAR(50) PRIMARY KEY,"
                                            "balance REAL NOT NULL"
                                            ");";

    const std::string CREATE_TABLE_CATEGORY =
        "CREATE TABLE IF NOT EXISTS Category ("
        "category_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name CHAR(50) NOT NULL"
        ");";

    const std::string CREATE_TABLE_WALLET_TRANSACTION =
        "CREATE TABLE IF NOT EXISTS WalletTransaction ("
        "wallet_transaction_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "wallet CHAR(50) NOT NULL,"
        "category_id INTEGER NOT NULL,"
        "type TEXT CHECK(type IN ('INCOME', 'EXPENSE')) NOT NULL,"
        "date DATE NOT NULL,"
        "amount REAL NOT NULL,"
        "description CHAR(50),"
        "FOREIGN KEY (wallet) REFERENCES Wallet(name),"
        "FOREIGN KEY (category_id) REFERENCES Category(category_id)"
        ");";

    const std::string CREATE_TABLE_TRANSFER =
        "CREATE TABLE IF NOT EXISTS Transfer ("
        "transfer_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "sender_wallet CHAR(50) NOT NULL,"
        "receiver_wallet CHAR(50) NOT NULL,"
        "date DATE NOT NULL,"
        "amount REAL NOT NULL,"
        "description CHAR(50),"
        "FOREIGN KEY (sender_wallet) REFERENCES Wallet(name),"
        "FOREIGN KEY (receiver_wallet) REFERENCES Wallet(name)"
        ");";

    const std::string CREATE_TABLE_CREDIT_CARD =
        "CREATE TABLE IF NOT EXISTS CreditCard ("
        "number CHAR(20) PRIMARY KEY,"
        "name CHAR(50) NOT NULL,"
        "max_debt REAL NOT NULL,"
        "billing_due_day INTEGER NOT NULL CHECK (billing_due_day >= 1 AND "
        "billing_due_day <= 28)"
        ");";

    const std::string CREATE_TABLE_CREDIT_CARD_DEBT =
        "CREATE TABLE IF NOT EXISTS CreditCardDebt ("
        "debt_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "crc_number CHAR(20) NOT NULL,"
        "category_id INTEGER NOT NULL,"
        "date DATE NOT NULL,"
        "total_amount REAL NOT NULL,"
        "description CHAR(50),"
        "FOREIGN KEY (crc_number) REFERENCES CreditCard(number)"
        "FOREIGN KEY (category_id) REFERENCES Category(category_id)"
        ");";

    const std::string CREATE_TABLE_CREDIT_CARD_PAYMENT =
        "CREATE TABLE IF NOT EXISTS CreditCardPayment ("
        "payment_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "wallet INTEGER,"
        "debt_id INTEGER NOT NULL,"
        "date DATE,"
        "amount REAL NOT NULL,"
        "installment INTEGER NOT NULL,"
        "FOREIGN KEY (wallet) REFERENCES Wallet(name),"
        "FOREIGN KEY (debt_id) REFERENCES CreditCardDebt(debt_id)"
        ");";

    // Queries to delete data from the database
    const std::string DELETE_TABLE_WALLET   = "DELETE FROM Wallet;";
    const std::string DELETE_TABLE_CATEGORY = "DELETE FROM Category;";
    const std::string DELETE_TABLE_WALLET_TRANSACTION =
        "DELETE FROM WalletTransaction;";
    const std::string DELETE_TABLE_TRANSFER         = "DELETE FROM Transfer;";
    const std::string DELETE_TABLE_CREDIT_CARD      = "DELETE FROM CreditCard;";
    const std::string DELETE_TABLE_CREDIT_CARD_DEBT = "DELETE FROM CreditCardDebt;";
    const std::string DELETE_TABLE_CREDIT_CARD_PAYMENT =
        "DELETE FROM CreditCardPayment;";

} // namespace query
#endif // SQL_QUERIES_H_
