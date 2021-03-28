//======== Copyright (c) 2021, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - Tests suite
//
// $NoKeywords: $ivs_project_1 $white_box_code.cpp
// $Author:     Hung Do <xdohun00@stud.fit.vutbr.cz>
// $Date:       $2021-01-04
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author Hung Do
 * 
 * @brief Implementace testu prace s maticemi.
 */

#include "gtest/gtest.h"
#include "white_box_code.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy operaci nad maticemi. Cilem testovani je:
// 1. Dosahnout maximalniho pokryti kodu (white_box_code.cpp) testy.
// 2. Overit spravne chovani operaci nad maticemi v zavislosti na rozmerech 
//    matic.
//============================================================================//

class InitMatrix : public ::testing::Test
{
    protected:
        Matrix *matrix;
        const int ROW = 2;
        const int COL = 2;
        const std::vector<std::vector<double>> VALUES = {
            { 3.5, 2.6 },
            { 1.2, -1.0 }
        };

        virtual void SetUp()
        {
            matrix = new Matrix(ROW, COL);
            matrix->set(VALUES);
        }

        virtual void TearDown()
        {
            delete matrix;
        }
};

TEST(UndefMatrix, FailedInit)
{
    // Spatna inicializace
    Matrix *m = nullptr;
    EXPECT_ANY_THROW(m = new Matrix(0, 0));
    delete m; m = nullptr;
    
    EXPECT_ANY_THROW(m = new Matrix(1, 0));
    delete m; m = nullptr;
    
    EXPECT_ANY_THROW(m = new Matrix(0, 1));
    delete m; m = nullptr;

    EXPECT_NO_THROW(m = new Matrix(1, 1));
    delete m;
}

TEST(UndefMatrix, SuccessInit)
{
    // Inicializace bez upresneni velikosti
    Matrix *mat1 = new Matrix();
    EXPECT_NE(mat1, nullptr);

    // Inicializace s upresnenim velikosti
    Matrix *mat2;
    EXPECT_NO_THROW(mat2 = new Matrix(3, 5));
    EXPECT_NE(mat2, nullptr);
    delete mat1;
    delete mat2;
}

TEST(UndefMatrix, InverseMatrix)
{
    Matrix *m1, *m2, *m3, res;
    // Inicializace jednotkovych matic
    Matrix *i2 = new Matrix(2, 2), *i3 = new Matrix(3, 3);
    i2->set({ {1, 0}, {0, 1} });
    i3->set({
        { 1, 0, 0 },
        { 0, 1, 0 },
        { 0, 0, 1 }
    });

    // Spatna matice
    m1 = new Matrix(2, 3);
    EXPECT_ANY_THROW(m1->inverse());
    delete m1;

    // Matice s determinanty rovne 0 a normalni
    // Matice 2x2
    m2 = new Matrix(2, 2);
    for (int r = 0; r < 2; r++)
        EXPECT_NO_THROW(m2->set(r, 0, r+1));
    EXPECT_ANY_THROW(m2->inverse());

    for (int r = 0; r < 2; r++)
        EXPECT_NO_THROW(m2->set(r, 1, r+2));

    EXPECT_NO_THROW(res = m2->inverse());
    EXPECT_TRUE((*i2 = res * *m2) == *m2 * res);

    // Matice 3x3
    m3 = new Matrix(3, 3);
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 2; c++)
            EXPECT_NO_THROW(m3->set(r, c, r+1-c));
    EXPECT_NO_THROW(m3->set(2, 0, -3));
    EXPECT_ANY_THROW(m3->inverse());

    EXPECT_NO_THROW(m3->set(2, 2, 1));
    EXPECT_NO_THROW(res = m3->inverse());
    EXPECT_TRUE((*i3 = res * *m3) == *m3 * res);
    delete i2; delete i3;
    delete m2; delete m3;
}

TEST(UndefMatrix, SolveEquation)
{
    std::vector<double> b0 = { 0, 0 }, b1 = { 4 }, b2 = { -8, -16 },
                        b3 = { 10, 20, 0 }, b4 = { 8, 4, 17, 10 };
    Matrix *m0 = new Matrix(5, 2);
    Matrix *m1 = new Matrix(1, 1);
    Matrix *m2 = new Matrix(2, 2);
    Matrix *m3 = new Matrix(3, 3);
    Matrix *m4 = new Matrix(4, 4);
    std::vector<double> res, expectVec;

    // Spatne podminky
    EXPECT_ANY_THROW(m0->solveEquation(b0));  // Matice neni ctvercova
    EXPECT_ANY_THROW(m2->solveEquation(b3));  // Velikost nesouhlasi 
    EXPECT_ANY_THROW(m2->solveEquation(b0));  // Nekonecne mnoho moznosti
    EXPECT_ANY_THROW(m2->solveEquation(b2));  // Zadne reseni

    // Reseni soustavy rovnic
    // Matice 1x1
    m1->set(0, 0, b1[0] / 2);
    EXPECT_NO_THROW(res = m1->solveEquation(b1));
    expectVec = { 2 };
    EXPECT_EQ(res, expectVec);

    // Matice 2x2
    m2->set({
        { -1,  3 },
        {  3, -1 }
    });
    EXPECT_NO_THROW(res = m2->solveEquation(b2));
    expectVec = { -7, -5 };
    EXPECT_EQ(res, expectVec);

    // Matice 3x3
    m3->set({
        { 2, 0, -1 },
        { 1, 3,  0 },
        { 0, 3, -2 }
    });
    EXPECT_NO_THROW(res = m3->solveEquation(b3));
    expectVec = {8, 4, 6};
    EXPECT_EQ(res, expectVec);

    // Matice 4x4
    m4->set({
        {1,  1,  1, 1},
        {1, -2, -1, 2},
        {2,  1,  1, 3},
        {3,  2, -1, 1}
    });
    EXPECT_NO_THROW(res = m4->solveEquation(b4));
    expectVec = {1, 2, 1, 4};
    EXPECT_EQ(res, expectVec);

    delete m0;
    delete m1; delete m2;
    delete m3; delete m4;
}

TEST(UndefMatrix, MultiplyByMatrix)
{
    // nasobeni maticemi 3x3 a 3x1
    Matrix *m1 = new Matrix(3, 3);
    Matrix *m2 = new Matrix(3, 1);
    Matrix *exp = new Matrix(3, 1);
    Matrix res;
    std::vector<std::vector<double>> expectVec = {
        { -1 },
        { -1 },
        { -1 }
    };

    EXPECT_NO_THROW(m1->set({
                { 1, 2, 3 },
                { 4, 5, 6 },
                { 7, 8, 9 }
    }));
    EXPECT_NO_THROW(m2->set({
                { 1 },
                {-1 },
                { 0 }
    }));
    EXPECT_NO_THROW(exp->set(expectVec));

    EXPECT_ANY_THROW(*m2 * *m1);
    EXPECT_NO_THROW(res = *m1 * *m2);
    EXPECT_TRUE(res == *exp);
    delete m1; delete m2;
    delete exp;
}

TEST_F(InitMatrix, SetValue_Single)
{
    ASSERT_NE(matrix, nullptr);

    // Kontrola vkladani po jednom prvku
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            if (r < ROW && c < COL)
                EXPECT_TRUE(matrix->set(r, c, 3.1415927));
            else
                EXPECT_FALSE(matrix->set(r, c, 3.1415927));
        }
    }
}

TEST_F(InitMatrix, SetValue_Vector)
{
    ASSERT_NE(matrix, nullptr);

    // Kontrola vlozeni prvku vektorem
    std::vector<std::vector<double>> values1 = { 
        { 1.0, 2.0 }, 
        { 1.5, 2.5 }
    };
    EXPECT_TRUE(matrix->set(values1));

    // Kontrola vlozeni vektoru s vetsim poctem sloupcu
    std::vector<std::vector<double>> values2 = {
        { 3, 5, 7 },
        { 2, 6, 8 }
    };
    EXPECT_FALSE(matrix->set(values2));

    // Kontrola vlozeni vektoru s vetsim poctem radku
    std::vector<std::vector<double>> values3 = {
        { 1, 2   },
        { 3, 4.0 },
        { 5, 6.0 }
    };
    EXPECT_FALSE(matrix->set(values3));

    // Kontrola vlozeni vektoru s mensim rozmerem
    std::vector<std::vector<double>> values4 = { { 1 },{ 2 } };
    EXPECT_FALSE(matrix->set(values4));
}

TEST_F(InitMatrix, GetValue)
{
    ASSERT_NE(matrix, nullptr);

    double value;
    // Kontrola pristupu do matice a vracene hodnoty
    EXPECT_NO_THROW(value = matrix->get(1, 1));
    EXPECT_EQ(value, VALUES[1][1]);

    EXPECT_NO_THROW(value = matrix->get(0, 1));
    EXPECT_EQ(value, VALUES[0][1]);

    EXPECT_ANY_THROW(value = matrix->get(2, 2));
}

TEST_F(InitMatrix, CompareMatrices)
{
    ASSERT_NE(matrix, nullptr);

    // Spatna velikost matic
    Matrix *m1 = new Matrix(3, 3);
    m1->set({ { 1, 2, 3 },
            { 4, 5, 6 },
            { 7, 8, 9 } });
    EXPECT_ANY_THROW(*matrix == *m1);
    delete m1;

    // Odlisne matice
    Matrix *m2 = new Matrix(2, 2);
    EXPECT_FALSE(*matrix == *m2);

    // Stejne matice
    EXPECT_NO_THROW(m2->set(VALUES));
    EXPECT_TRUE(*matrix == *m2);
    delete m2;
}

TEST_F(InitMatrix, MatrixAddition)
{
    ASSERT_NE(matrix, nullptr);

    // Spatna velikost matic
    Matrix *m1 = new Matrix(3, 3);
    m1->set({ { 1, 2, 3 },
            { 4, 5, 6 },
            { 7, 8, 9} });
    EXPECT_ANY_THROW(*matrix + *m1);
    delete m1;

    // Spravne secteni
    Matrix *m2 = new Matrix(ROW, COL);
    EXPECT_NO_THROW(m2->set({ { 0.5, -0.6 },
                              { -2 ,  1.5 } }));
    Matrix *expectMat = new Matrix(ROW, COL);
    EXPECT_NO_THROW(expectMat->set({ {  4.0, 2.0 },
                                     { -0.8, 0.5 } }));
    Matrix res;
    EXPECT_NO_THROW(res = *matrix + *m2);
    EXPECT_TRUE(*expectMat == res);
    delete m2;
    delete expectMat;
}

TEST_F(InitMatrix, MultiplyByConstant)
{
    ASSERT_NE(matrix, nullptr);

    // Nasobeni nulou
    Matrix *expectMat = new Matrix(ROW, COL);
    Matrix res;

    expectMat->set({ { 0, 0 }, { 0, 0 } });
    EXPECT_NO_THROW(res = *matrix * 0.0);
    EXPECT_TRUE(res == *expectMat);

    // Nasobeni zapornym cislem
    for (int r = 0; r < ROW; r++)
        for (int c = 0; c < COL; c++)
            expectMat->set(r, c, -2.0 * VALUES[r][c]);
    EXPECT_NO_THROW(res = *matrix * -2.0);
    EXPECT_TRUE(res == *expectMat);

    // Nasobeni kladnym cislem
    for (int r = 0; r < ROW; r++)
        for (int c = 0; c < COL; c++)
            expectMat->set(r, c, 3.0 * VALUES[r][c]);
    EXPECT_NO_THROW(res = *matrix * 3.0);
    EXPECT_TRUE(res == *expectMat);

    delete expectMat;
}

TEST_F(InitMatrix, MultiplyByMatrix)
{
    ASSERT_NE(matrix, nullptr);

    // Kontrola chybnych stavu
    Matrix *m1 = new Matrix(3, 3);
    EXPECT_ANY_THROW(*m1 * *matrix);
    delete m1; m1 = nullptr;

    m1 = new Matrix(2, 3);
    EXPECT_ANY_THROW(*m1 * *matrix);
    EXPECT_NO_THROW(*matrix * *m1);
    delete m1; m1 = nullptr;

    m1 = new Matrix(3, 2);
    EXPECT_ANY_THROW(*matrix * *m1);
    EXPECT_NO_THROW(*m1 * *matrix);
    delete m1;

    // Kontrola vypoctu
    Matrix *m2 = new Matrix(ROW, COL);
    Matrix *expectMat = new Matrix(ROW, COL);
    Matrix res;

    // Nasobeni jednotkovou matici
    expectMat->set(VALUES);
    m2->set({ { 1, 0 }, { 0, 1 } });
    EXPECT_NO_THROW(res = *matrix * *m2);
    EXPECT_TRUE(res == *expectMat);

    matrix->set({ { 1, 2 }, { 4, -3 } });
    m2->set({ { -2, -6 }, { 5, 0 } });
    expectMat->set({ { 8, -6 }, { -23, -24 } });
    EXPECT_NO_THROW(res = *matrix * *m2);
    EXPECT_TRUE(res == *expectMat);
    delete expectMat;
    delete m2;
}

TEST_F(InitMatrix, Transpose)
{
    ASSERT_NE(matrix, nullptr);

    // Kontrola transponovane matice
    Matrix *expectMat = new Matrix(COL, ROW);
    for (int r = 0; r < COL; r++)
        for (int c = 0; c < ROW; c++)
            EXPECT_NO_THROW(expectMat->set(r, c, VALUES[c][r]));
    
    EXPECT_TRUE(*expectMat == matrix->transpose());
    delete expectMat;
}

/*** Konec souboru white_box_tests.cpp ***/
