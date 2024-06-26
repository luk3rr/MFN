/*
 * Filename: credit_card_manager.h
 * Created on: June 11, 2024
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 *
 * This file contains the declaration of the CreditCardManager class.
 * This class is responsible for managing the credit cards in the database.
 */

#ifndef CREDIT_CARD_MANAGER_H_
#define CREDIT_CARD_MANAGER_H_

#include "category_manager.h"
#include "db_manager.h"
#include "log_manager.h"
#include "utils.h"
#include <cmath>
#include <cstdint>

/**
 * @brief The CreditCardManager class is responsible for managing the credit cards in
 *the database. It is responsible for adding new credit cards, adding new expenses to
 *the credit cards and getting the credit card information.
 **/
class CreditCardManager
{
    private:
        DBManager&      m_dbManager;
        LogManager&     m_logManager;
        CategoryManager m_categoryManager;

    public:
        /**
         * @brief Default constructor
         **/
        CreditCardManager() noexcept;

        /**
         * @brief Default destructor
         **/
        ~CreditCardManager() noexcept;

        /**
         * @brief Get the credit cards in the database
         * @param creditCards The vector to store the credit cards numbers
         * NOTE: All data stored in the vector will be lost
         **/
        void GetCreditCards(std::vector<std::string>& creditCards) const noexcept;

        /**
         * @brief Get the credit card information
         * @param cardNumber The credit card number
         * @param cardName The name of the credit card
         * @param maxDebt The maximum debt allowed for the credit card
         * @param totalPendingDebt The total pending debt of the credit card
         * @param billingDueDay The day of the month the bill is due
         * @return True if the credit card exists, false otherwise
         **/
        bool GetCreditCardInfo(const std::string& cardNumber,
                               std::string&       cardName,
                               double_t&          maxDebt,
                               double_t&          totalPendingDebt,
                               uint16_t&          billingDueDay) const noexcept;

        /**
         * @brief Add a new credit card to the database
         * @param cardNumber The credit card number
         * @param billingDueDay The day of the month the bill is due
         * @param cardName The name of the credit card
         * @param maxDebt The maximum debt allowed for the credit card
         **/
        bool AddCreditCard(const std::string& cardNumber,
                           const uint16_t     billingDueDay,
                           const std::string& cardName,
                           const double_t     maxDebt) noexcept;

        /**
         * @brief Add a new expense to the credit card
         * @param cardNumber The credit card number
         * @param category The category of the expense
         * @param date The date of the expense
         * @param totalAmount The total amount of the expense
         * @param description The description of the expense
         * @param installments The number of installments of the expense
         **/
        bool AddDebt(const std::string& cardNumber,
                     const std::string& category,
                     const std::string& date,
                     const double_t     totalAmount,
                     const std::string& description,
                     const uint16_t     installments) noexcept;

        /**
         * @brief Get information about the last expense of a credit card
         * @param cardNumber The credit card number
         * @param category The category of the expense
         * @param date The date of the expense
         * @param totalAmount The total amount of the expense
         * @param description The description of the expense
         * @param installments The number of installments of the expense
         * @param debtId The id of the expense
         * @return True if the credit card has expenses, false otherwise
         **/
        bool GetLastExpense(const std::string& cardNumber,
                            std::string&       category,
                            std::string&       date,
                            double_t&          totalAmount,
                            std::string&       description,
                            uint16_t&          installments,
                            uint32_t&          debtId) const noexcept;

    private:
        /**
         * @brief Check if a credit card exists in the database
         * @param cardNumber The credit card number
         * @return True if the credit card exists, false otherwise
         **/
        bool CreditCardExists(const std::string& cardNumber) const noexcept;

        /**
         * @brief Get the maximum debt allowed for a credit card
         * @param cardNumber The credit card number
         * @throw std::runtime_error if the credit card does not exist
         **/
        double_t GetMaxDebt(const std::string& cardNumber) const;

        /**
         * @brief Get the total pending debt of a credit card
         * @param cardNumber The credit card number
         * @throw std::runtime_error if the credit card does not exist
         **/
        double_t GetTotalPendingDebt(const std::string& cardNumber) const;

        /**
         * @brief Check if a credit card has enough credit to make a purchase
         * @param cardNumber The credit card number
         * @throw std::runtime_error if the credit card does not exist
         **/
        bool HasEnoughCredit(const std::string& cardNumber,
                             const double_t     expense) const;

        /**
         * @brief Generate the due date of a installment
         * @param cardNumber The credit card number
         * @param purchaseDate The date of the purchase
         * @param installmentNumber The number of the installment
         * @return The due date of the installment
         * @throw std::runtime_error if the credit card does not exist
         **/
        std::string GetInstallmentDueDate(const std::string& cardNumber,
                                          const std::string& purchaseDate,
                                          uint16_t           installmentNumber) const;
};

#endif // CREDIT_CARD_MANAGER_H_
