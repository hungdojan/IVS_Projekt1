//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     Hung Do <xdohun00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author Hung Do
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>
#include <climits>
#include <string>

#include "gtest/gtest.h"

#include "red_black_tree.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//

class EmptyTree : public ::testing::Test
{
protected:
    BinaryTree *tree;

    virtual void SetUp() 
    {
        tree = new BinaryTree();
    }

    virtual void TearDown() 
    {
        delete tree;
    }
};

class NonEmptyTree : public ::testing::Test
{
protected:
    BinaryTree *tree;
    const static int length = 7;
    int keys[length] = { -9, -2, 1, 5, 7, 9, 10 };
    virtual void SetUp() 
    {
        tree = new BinaryTree();
        for (const int &item : keys)
            tree->InsertNode(item);
    }

    virtual void TearDown() 
    {
        delete tree;
    }
};

class TreeAxioms : public ::testing::Test
{
protected:
    BinaryTree *tree;

    virtual void SetUp() 
    {
        tree = new BinaryTree();
        /*
         *  Kostra stromu
         *       2
         *      / \
         *     1   5
         */
        tree->InsertNode(2);
        tree->InsertNode(1);
        tree->InsertNode(5);
    }

    virtual void TearDown() 
    {
        delete tree;
    }
};

TEST_F(EmptyTree, FindNode) 
{
    // Hledani uzlu v prazdneho stromu
    // otestovani pro -1, 0 a 1
    for (int i = -1; i <= 1; i++)
        EXPECT_TRUE(tree->FindNode(i) == NULL);
}

TEST_F(EmptyTree, DeleteNode) 
{
    // Odstraneni uzlu v prazdnem strome
    // testovani pro klice -1, 0, 1
    for (int i = -1; i <= 1; i++)
        EXPECT_FALSE(tree->DeleteNode(i));
}

TEST_F(EmptyTree, InsertNode) 
{
    // Prida nekolik uzlu do prazdneho stromu
    std::pair<bool, BinaryTree::Node_t *> temp;

    // Testovani pro klice -1, 0 a 1
    for (int i = -1; i < 1; i++) 
    {
        EXPECT_NO_THROW(temp = tree->InsertNode(i));
        ASSERT_TRUE(temp.first);
        ASSERT_FALSE(temp.second == NULL);
    }
    EXPECT_FALSE(tree->GetRoot() == NULL);
}

//=============================================

TEST_F(NonEmptyTree, InsertNode) 
{
    // Prida nekolik uzlu do neprazdneho stromu
    // Nahlasi false pokud se uzel jiz nachazi ve strome
    int index = 0;
    std::pair<bool, BinaryTree::Node_t *> temp;
    // Pruchod intervalem <-10; 10>
    for (int i = -10; i <= 10; i++) 
    {
        EXPECT_NO_THROW(temp = tree->InsertNode(i));
        ASSERT_TRUE(temp.second != NULL);
        if (index < length) 
        {
            // pokud se uzel jiz nachazi ve strome
            // pole ocekavanych uzlu musi byt jiz serazene
            if (i == keys[index]) 
            {
                EXPECT_FALSE(temp.first);
                index++;
            } 
            else 
                EXPECT_TRUE(temp.first);
        }
        else
            EXPECT_TRUE(temp.first);
        EXPECT_EQ(temp.second->key, i);
    }
}

TEST_F(NonEmptyTree, DeleteNode_Random) 
{
    // Odstrani nektere uzly ve strome
    // keys[] = { -9, -2, 1, 5, 7, 9, 10 };

    // Odstraneni uzlu, ktere se nenachazi ve strome
    EXPECT_FALSE(tree->DeleteNode(-15));
    EXPECT_FALSE(tree->DeleteNode(15));
    EXPECT_FALSE(tree->DeleteNode(0));
    EXPECT_FALSE(tree->DeleteNode(INT_MAX));
    EXPECT_FALSE(tree->DeleteNode(INT_MIN));

    // Odstraneni uzlu, kter 
    EXPECT_TRUE(tree->DeleteNode(1));
    EXPECT_TRUE(tree->DeleteNode(-9));
    EXPECT_TRUE(tree->DeleteNode(10));

    // Odstraneni jiz odstranenych uzlu
    EXPECT_FALSE(tree->DeleteNode(-9));
    EXPECT_FALSE(tree->DeleteNode(10));

    // Pokud smazu koren, tak se nesmaze cely strom
    Node_t *root;
    EXPECT_NO_THROW(root = tree->GetRoot());
    int key = root->key;
    EXPECT_TRUE(tree->DeleteNode(root->key));
    EXPECT_TRUE(tree->GetRoot() != NULL);
    EXPECT_NE(tree->GetRoot()->key, key);
}

TEST_F(NonEmptyTree, DeleteNode_All) 
{
    // Odstrani vsechny uzly ve strome
    for (int i = 0; i < length; i++)
        EXPECT_TRUE(tree->DeleteNode(keys[i]));

    for (int i = 0; i < length; i++)
        EXPECT_TRUE(tree->FindNode(keys[i]) == NULL);
    EXPECT_TRUE(tree->GetRoot() == NULL);
}

TEST_F(NonEmptyTree, FindNode) 
{
    // Prohleda uzly ve strome
    int index = 0;
    // Pruchod intervalem <-100; 100>
    for (int i = -100; i <= 100; i++) 
    {
        if (index < length) 
        {
            // pole ocekavanych uzlu musi byt jiz serazene
            if (i == keys[index]) 
            {
                ASSERT_TRUE(tree->FindNode(i));
                index++;
            } 
            else
                ASSERT_FALSE(tree->FindNode(i));
        }
        else
            ASSERT_FALSE(tree->FindNode(i));
    }
}

// Vsechny listove uzly jsou cerne
TEST_F(TreeAxioms, Axiom1) 
{
    std::vector<BinaryTree::Node_t *> outLeafNodes;
    EXPECT_NO_THROW(tree->GetLeafNodes(outLeafNodes));

    for (const Node_t *leaf : outLeafNodes)
    {
        // Chyba pokud se nejedna o list
        ASSERT_TRUE(leaf->pRight == NULL);
        ASSERT_TRUE(leaf->pLeft == NULL);
        EXPECT_EQ(leaf->color, Color_t::BLACK);
    }
}

// Pokud je uzel cerveny, pak jsou jeho oba potomci cerni
TEST_F(TreeAxioms, Axiom2) 
{
    std::vector<BinaryTree::Node_t *> outAllNodes;
    EXPECT_NO_THROW(tree->GetAllNodes(outAllNodes));
    for (const Node_t *node : outAllNodes) 
    {
        if (node->color == Color_t::RED) 
        {
            if (node->pParent != NULL)
                EXPECT_NE(node->pParent->color, Color_t::RED);
            EXPECT_EQ(node->pLeft->color, Color_t::BLACK);
            EXPECT_EQ(node->pRight->color, Color_t::BLACK);
        }
    }
}

// Kazda cesta od kazdeho listoveho uzlu ke koreni obsahuje
// stejny pocet cernych uzlu
TEST_F(TreeAxioms, Axiom3) 
{
    std::vector<BinaryTree::Node_t *> outLeafNodes;
    int blackNodes = 0;

    EXPECT_NO_THROW(tree->GetLeafNodes(outLeafNodes));
    for (Node_t *leaf : outLeafNodes) 
    {
        // Chyba pokud se nejedna o list
        ASSERT_TRUE(leaf->pRight == NULL);
        ASSERT_TRUE(leaf->pLeft == NULL);
        EXPECT_EQ(leaf->color, Color_t::BLACK);

        // Prvni pruchod; nacteni poctu cernych uzlu
        // jelikoz je koren vzdy cerny, bude hodnota blackNodes > 0
        if (!blackNodes) 
        {
            // Pruchod ke koreni
            for (Node_t *temp = leaf; temp != NULL; temp = temp->pParent) 
            {
                if (temp->color == Color_t::BLACK) 
                    blackNodes++;
            }
        }
        // kontrola ostatnich uzlu, zda cesta ke koreni obsahuje
        // stejny pocet cernych uzlu
        else
        {
            int metBlackNodes = 0;       // pocet cernych uzlu na ceste
            // Pruchod ke koreni
            for (Node_t *temp = leaf; temp != NULL; temp = temp->pParent)
            {
                if (temp->color == Color_t::BLACK) 
                    metBlackNodes++;
            }
            EXPECT_EQ(metBlackNodes, blackNodes);
        }
    }
}

/*** Konec souboru black_box_tests.cpp ***/
