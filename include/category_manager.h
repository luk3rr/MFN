/*
 * Filename: category_manager.h
 * Created on: June 13, 2024
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 *
 * This file contains the declaration of the CategoryManager class.
 * This class is responsible for managing the categories of the application.
 */

#ifndef CATEGORY_MANAGER_H_
#define CATEGORY_MANAGER_H_

#include "db_manager.h"
#include "log_manager.h"
#include <cstddef>
#include <string>

/**
 * @brief The CategoryManager class is responsible for managing the categories of the
 *application.
 **/
class CategoryManager
{
    private:
        DBManager&  m_dbManager;
        LogManager& m_logManager;

    public:
        /**
         * @brief Default constructor
         **/
        CategoryManager();

        /**
         * @brief Default destructor
         **/
        ~CategoryManager();

        /**
         * @brief Get all categories names
         * @param categories A vector to store the categories names
         * NOTE: The vector will be cleared before storing the names
         **/
        void GetCategoriesNames(std::vector<std::string>& categories) const noexcept;

        /**
         * @brief Get a category by its name
         * @param name The name of the category
         * @return The category id
         * @throw runtime_error If the category does not exist
         **/
        std::size_t GetCategoryID(const std::string& name) const;

        /**
         * @brief Create a new category
         * @param name The name of the category
         * @return True if the category was created, false otherwise
         **/
        bool CreateCategory(const std::string& name) noexcept;

        /**
         * @brief Check if a category exists
         * @param name The name of the category
         * @return True if the category exists, false otherwise
         **/
        bool CategoryExists(const std::string& name) const noexcept;
};

#endif // CATEGORY_MANAGER_H_
