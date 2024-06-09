/*
 * Filename: wallet.h
 * Created on: June  8, 2024
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 *
 * This file contains the Wallet class responsible for managing the wallet
 * operations.
 *
 * The Wallet class is responsible for managing the wallet operations, such as
 * creating a new wallet, deleting a wallet, registering a new expense, and
 * registering a new income.
 */

#ifndef WALLET_H_
#define WALLET_H_

#include <cmath>
#include <string>

#include "db_manager.h"
#include "log_manager.h"

/**
 * @brief The Wallet class is responsible for managing the wallet operations
 * such as creating a new wallet, deleting a wallet, registering a new expense,
 * and registering a new income.
 **/
class Wallet
{
    private:
        LogManager& m_logManager;
        DBManager&  m_dbManager;

    public:
        /**
         * @brief Default constructor
         **/
        Wallet() noexcept;

        /**
         * @brief Default destructor
         **/
        ~Wallet() noexcept;

        /**
         * @brief Create a new wallet
         * @param walletName The wallet name
         **/
        void CreateWallet(const std::string& walletName,
                          const double_t     initialBalance = 0) noexcept;

        /**
         * @brief Delete a wallet
         * @param walletName The wallet name
         **/
        void DeleteWallet(const std::string& walletName) noexcept;

        /**
         * @brief Register a new expense
         * @param walletName The wallet name
         * @param category The expense category
         * @param date The expense date
         * @param description The expense description
         * @param amount The expense amount
         **/
        void Expense(const std::string& walletName,
                     const std::string& category,
                     const std::string& date,
                     const std::string& description,
                     const double_t     amount) noexcept;

        /**
         * @brief Register a new income
         * @param walletName The wallet name
         * @param category The income category
         * @param date The income date
         * @param description The income description
         * @param amount The income amount
         **/
        void Income(const std::string& walletName,
                    const std::string& category,
                    const std::string& date,
                    const std::string& description,
                    const double_t     amount) noexcept;

        /**
         * @brief Transfer money between wallets
         * @param srcWalletId The source wallet id
         * @param dstWalletId The destination wallet id
         * @param date The transfer date
         * @param amount The amount to be transferred
         **/
        void Transfer(const std::string& srcWalletId,
                      const std::string& dstWalletId,
                      const std::string& date,
                      const double_t&    amount) noexcept;

    private:
        /**
         * @brief Check if a wallet exists
         * @param walletName The wallet name
         * @return True if the wallet exists, false otherwise
         **/
        bool WalletExists(const std::string& walletName) noexcept;

        /**
         * @brief Update the wallet balance
         * @param walletName The wallet name
         * @param amount The amount to be updated
         **/
        void UpdateBalance(const std::string& walletName,
                           const double_t     amount) noexcept;

        /**
         * @brief Check if a category exists
         * @param category The category name
         * @return True if the category exists, false otherwise
         **/
        bool CategoryExists(const std::string& category) noexcept;

        /**
         * @brief Create a new category
         * @param category The category name
         **/
        void CreateCategory(const std::string& category) noexcept;

        /**
         * @brief Get the category id
         * @param category The category name
         * @return The category id
         **/
        std::size_t GetCategoryID(const std::string& category);

        /**
         * @brief Get the wallet balance
         * @param walletName The wallet name
         * @return The wallet balance
         **/
        double_t GetBalance(const std::string& walletName) noexcept;
};

#endif // WALLET_H_
