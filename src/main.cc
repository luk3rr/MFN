/*
 * Filename: main.cc
 * Created on: June  7, 2024
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 */

#include <iostream>
#include <stdexcept>

#include "config.h"
#include "db_manager.h"
#include "log_manager.h"
#include "wallet.h"

int main()
{
    LogManager& logger = LogManager::GetInstance();
    try
    {
        DBManager& db = DBManager::GetInstance();

        Wallet wallet;

        wallet.CreateWallet("carteira", 33.25);
        wallet.CreateWallet("bancoDaEsquina");

        wallet.Expense("bancoDaEsquina", "home", "2024/06/08", "gasto com a casa", 50);
        wallet.Expense("carteira", "food", "2024/06/09", "dogao", 17.70);
        wallet.Income("carteira", "salary", "2024/06/08", "salario", 1000);
        wallet.Expense("bancoDaEsquina", "home", "2024/06/08", "gasto com a casa", 50);

        wallet.Expense("bancoDaEsquina", "home", "2024/06/09", "gasto com a casa", 50);
        wallet.Income("bancoDaEsquina", "salary", "2024/06/09", "salario", 1000);
        wallet.Expense("bancoDaEsquina", "home", "2024/06/09", "gasto com a casa", 50);
    }
    catch (const std::runtime_error& re)
    {
        std::cerr << re.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
