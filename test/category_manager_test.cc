/*
 * Filename: category_manager_test.cc
 * Created on: June 13, 2024
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 */

#include <cstdint>
#include <cstdlib>
#include <gtest/gtest.h>

#include "category_manager.h"
#include "db_manager.h"

class CategoryManagerTest : public testing::Test
{
    protected:
        DBManager&       m_dbManager;
        CategoryManager* m_categoryManager;

        CategoryManagerTest()
            : m_dbManager(DBManager::GetInstance())
        { }

        void SetUp() override
        {
            m_dbManager.ResetDatabase();
            m_categoryManager = new CategoryManager();
        }

        void TearDown() override
        {
            delete m_categoryManager;
        }
};

TEST_F(CategoryManagerTest, AddCategory)
{
    bool add1 = m_categoryManager->CreateCategory("C1");
    bool add2 = m_categoryManager->CreateCategory("C2");
    bool add3 = m_categoryManager->CreateCategory("C3");

    ASSERT_TRUE(add1);
    ASSERT_TRUE(add2);
    ASSERT_TRUE(add3);

    std::vector<std::string> categories;

    m_categoryManager->GetCategoriesNames(categories);

    ASSERT_EQ(3, categories.size());
    ASSERT_EQ("C2", categories[1]);
}

TEST_F(CategoryManagerTest, AddCategoryWithSameName)
{
    bool add1 = m_categoryManager->CreateCategory("C1");
    bool add2 = m_categoryManager->CreateCategory("C2");
    bool add3 = m_categoryManager->CreateCategory("C1");

    ASSERT_TRUE(add1);
    ASSERT_TRUE(add2);
    ASSERT_FALSE(add3);

    std::vector<std::string> categories;

    m_categoryManager->GetCategoriesNames(categories);

    ASSERT_EQ(2, categories.size());
    ASSERT_EQ("C2", categories[1]);
}

TEST_F(CategoryManagerTest, GetCategoryIDWithInvalidName)
{
    bool add1 = m_categoryManager->CreateCategory("C1");
    bool add2 = m_categoryManager->CreateCategory("C2");
    bool add3 = m_categoryManager->CreateCategory("C3");

    ASSERT_TRUE(add1);
    ASSERT_TRUE(add2);
    ASSERT_TRUE(add3);

    ASSERT_THROW(m_categoryManager->GetCategoryID("C4"), std::runtime_error);
}
