/*
 * Filename: wallet_manager_test.cc
 * Created on: June 10, 2024
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 */

#include <cmath>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>

#include "db_manager.h"
#include "wallet_manager.h"

class WalletManagerTest : public testing::Test
{
    protected:
        WalletManager* m_walletManager;
        DBManager&     m_dbManager;

        void SetUp() override
        {
            m_dbManager.ResetDatabase();
            m_walletManager = new WalletManager();
        }

        void TearDown() override { }

    public:
        WalletManagerTest()
            : m_dbManager(DBManager::GetInstance())
        { }

        ~WalletManagerTest()
        {
            delete m_walletManager;
        }
};

TEST_F(WalletManagerTest, CreateValidWallet)
{
    m_walletManager->CreateWallet("w1");
    m_walletManager->CreateWallet("w2");
    m_walletManager->CreateWallet("w3");

    std::vector<std::string> wallets;

    m_walletManager->GetWallets(wallets);

    EXPECT_EQ(3, wallets.size());

    m_walletManager->CreateWallet("w4", 100);

    m_walletManager->GetWallets(wallets);

    EXPECT_EQ(4, wallets.size());
}

TEST_F(WalletManagerTest, CreateInvalidWallet)
{
    m_walletManager->CreateWallet("w1");
    m_walletManager->CreateWallet("w2");
    m_walletManager->CreateWallet("w3");

    std::vector<std::string> wallets;

    m_walletManager->GetWallets(wallets);

    EXPECT_EQ(3, wallets.size());

    m_walletManager->CreateWallet("w2");

    m_walletManager->GetWallets(wallets);

    EXPECT_EQ(3, wallets.size());
}

TEST_F(WalletManagerTest, DeleteValidWallet)
{
    m_walletManager->CreateWallet("w1");
    m_walletManager->CreateWallet("w2");
    m_walletManager->CreateWallet("w3");

    std::vector<std::string> wallets;

    m_walletManager->GetWallets(wallets);

    EXPECT_EQ(3, wallets.size());

    m_walletManager->DeleteWallet("w2");

    m_walletManager->GetWallets(wallets);

    EXPECT_EQ(2, wallets.size());
    EXPECT_EQ("w1", wallets[0]);
    EXPECT_EQ("w3", wallets[1]);
}

TEST_F(WalletManagerTest, DeleteInvalidWallet)
{
    m_walletManager->CreateWallet("w1");
    m_walletManager->CreateWallet("w2");
    m_walletManager->CreateWallet("w3");

    std::vector<std::string> wallets;

    m_walletManager->GetWallets(wallets);

    EXPECT_EQ(3, wallets.size());

    m_walletManager->DeleteWallet("w4");

    m_walletManager->GetWallets(wallets);

    EXPECT_EQ(3, wallets.size());
}

TEST_F(WalletManagerTest, IncomeValidValue)
{
    m_walletManager->CreateWallet("w1", 100);
    m_walletManager->CreateWallet("w2", 200);

    m_walletManager->Income("w1", "", "", "", 50);
    m_walletManager->Income("w2", "", "", "", 100);

    std::vector<std::string> wallets;
    std::vector<double_t>    balances;

    m_walletManager->GetWallets(wallets, balances);

    EXPECT_EQ(2, wallets.size());
    EXPECT_EQ(150, balances[0]);
    EXPECT_EQ(300, balances[1]);
}

TEST_F(WalletManagerTest, IncomeInvalidValue)
{
    m_walletManager->CreateWallet("w1", 100);
    m_walletManager->CreateWallet("w2", 200);

    m_walletManager->Income("w1", "", "", "", -50);
    m_walletManager->Income("w2", "", "", "", 0);

    std::vector<std::string> wallets;
    std::vector<double_t>    balances;

    m_walletManager->GetWallets(wallets, balances);

    EXPECT_EQ(2, wallets.size());
    EXPECT_EQ(100, balances[0]);
    EXPECT_EQ(200, balances[1]);
}

TEST_F(WalletManagerTest, ExpenseValidValue)
{
    m_walletManager->CreateWallet("w1", 100);
    m_walletManager->CreateWallet("w2", 200);

    m_walletManager->Expense("w1", "", "", "", 50);
    m_walletManager->Expense("w2", "", "", "", 100);

    std::vector<std::string> wallets;
    std::vector<double_t>    balances;

    m_walletManager->GetWallets(wallets, balances);

    EXPECT_EQ(2, wallets.size());
    EXPECT_EQ(50, balances[0]);
    EXPECT_EQ(100, balances[1]);
}

TEST_F(WalletManagerTest, ExpenseInvalidValue)
{
    m_walletManager->CreateWallet("w1", 100);
    m_walletManager->CreateWallet("w2", 200);

    m_walletManager->Expense("w1", "", "", "", -50);
    m_walletManager->Expense("w2", "", "", "", 0);

    std::vector<std::string> wallets;
    std::vector<double_t>    balances;

    m_walletManager->GetWallets(wallets, balances);

    EXPECT_EQ(2, wallets.size());
    EXPECT_EQ(100, balances[0]);
    EXPECT_EQ(200, balances[1]);
}

TEST_F(WalletManagerTest, TransferValidValue)
{
    m_walletManager->CreateWallet("w1", 100);
    m_walletManager->CreateWallet("w2", 200);

    m_walletManager->Transfer("w1", "w2", "", 50);

    std::vector<std::string> wallets;
    std::vector<double_t>    balances;

    m_walletManager->GetWallets(wallets, balances);

    EXPECT_EQ(2, wallets.size());
    EXPECT_EQ(50, balances[0]);
    EXPECT_EQ(250, balances[1]);
}

TEST_F(WalletManagerTest, TransferInvalidValue)
{
    m_walletManager->CreateWallet("w1", 100);
    m_walletManager->CreateWallet("w2", 200);

    m_walletManager->Transfer("w1", "w2", "", -50);
    m_walletManager->Transfer("w1", "w2", "", 0);

    std::vector<std::string> wallets;
    std::vector<double_t>    balances;

    m_walletManager->GetWallets(wallets, balances);

    EXPECT_EQ(2, wallets.size());
    EXPECT_EQ(100, balances[0]);
    EXPECT_EQ(200, balances[1]);
}
