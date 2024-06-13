/*
 * Filename: category_manager.cc
 * Created on: June 13, 2024
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 */

#include "category_manager.h"
#include <vector>

CategoryManager::CategoryManager()
    : m_dbManager(DBManager::GetInstance()),
      m_logManager(LogManager::GetInstance())
{ }

CategoryManager::~CategoryManager() { }

void CategoryManager::GetCategoriesNames(
    std::vector<std::string>& categories) const noexcept
{
    categories.clear();

    std::string query = "SELECT name FROM Category;";

    this->m_dbManager.ExecuteQueryWithResult(query, [&categories](sqlite3_stmt* stmt) {
        categories.push_back(
            std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))));
    });
}

std::size_t CategoryManager::GetCategoryID(const std::string& category) const
{
    // Check if category exists
    if (not this->CategoryExists(category))
    {
        throw std::runtime_error("Category does not exist.");
    }

    std::string query =
        fmt::format("SELECT category_id FROM Category WHERE name = '{}';", category);

    std::size_t id = 0;

    this->m_dbManager.ExecuteQueryWithResult(query, [&id](sqlite3_stmt* stmt) {
        id = sqlite3_column_int64(stmt, 0);
    });

    return id;
}

bool CategoryManager::CreateCategory(const std::string& name) noexcept
{
    std::string query = fmt::format("INSERT INTO Category (name) VALUES ('{}');", name);

    if (this->CategoryExists(name))
    {
        this->m_logManager.Log("Category '" + name + "' already exists.");
        return false;
    }
    else
    {
        this->m_dbManager.ExecuteQuery(query);
        this->m_logManager.Log("Category '" + name + "' created.");
        return true;
    }
}

bool CategoryManager::CategoryExists(const std::string& name) const noexcept
{
    std::string query =
        fmt::format("SELECT COUNT(*) FROM Category WHERE name = '{}';", name);

    int count = 0;

    this->m_dbManager.ExecuteQueryWithResult(query, [&count](sqlite3_stmt* stmt) {
        count = sqlite3_column_int(stmt, 0);
    });

    return count > 0;
}
