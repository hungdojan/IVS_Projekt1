//======== Copyright (c) 2021, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - priority queue code
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     Hung Do <xdohun00@stud.fit.vutbr.cz>
// $Date:       $2021-01-04
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author Hung Do
 * 
 * @brief Implementace metod tridy prioritni fronty.
 */

#include <stdlib.h>
#include <stdio.h>

#include "tdd_code.h"

//============================================================================//
// ** ZDE DOPLNTE IMPLEMENTACI **
//
// Zde doplnte implementaci verejneho rozhrani prioritni fronty (Priority Queue)
// 1. Verejne rozhrani fronty specifikovane v: tdd_code.h (sekce "public:")
//    - Konstruktor (PriorityQueue()), Destruktor (~PriorityQueue())
//    - Metody Insert/Remove/Find a GetHead
//    - Pripadne vase metody definovane v tdd_code.h (sekce "protected:")
//
// Cilem je dosahnout plne funkcni implementace prioritni fronty implementovane
// pomoci tzv. "double-linked list", ktera bude splnovat dodane testy 
// (tdd_tests.cpp).
//============================================================================//

PriorityQueue::PriorityQueue()
{
    m_pHead = nullptr;
}

PriorityQueue::~PriorityQueue()
{
    if (m_pHead != nullptr)
    {
        Element_t *temp = m_pHead;
        while (m_pHead != nullptr)
        {
            m_pHead = m_pHead->pNext;
            delete temp;
            temp = m_pHead;
        }
    }
}


void PriorityQueue::Insert(int value)
{
    // Inicializace elementu
    Element_t *element = new Element_t;
    if (element != nullptr)
    {
        element->pNext = nullptr;
        element->value = value;
    }

    // Prazdna fronta
    if (m_pHead == nullptr)
        m_pHead = element;

    // Vkladani elementu na zacatek fronty (pozice m_pHead)
    else if (m_pHead->value <= element->value)
    {
        Element_t *temp = m_pHead;
        m_pHead = element;
        element->pNext = temp;
    }
    else
    {
        Element_t *curr;
        // Hledani elementu, ZA ktery se vlozi novy element
        for (curr = m_pHead; curr->pNext != nullptr &&
                             curr->pNext->value > value; curr = curr->pNext)
            ;       // zamerne zastaveni
        // Vkladani za posledni element
        if (curr->pNext == nullptr)
            curr->pNext = element;
        else        // Vkladani uprostred
        {
            Element_t *temp = curr->pNext;
            curr->pNext = element;
            element->pNext = temp;
        }
    }
}

bool PriorityQueue::Remove(int value)
{
    if (m_pHead != nullptr)
    {
        // Mazani prvniho elementu (pozice m_pHead)
        if (m_pHead->value == value)
        {
            Element_t *temp = m_pHead->pNext;
            delete m_pHead;
            m_pHead = temp;
            return true;
        }
        else
        {
            // Hledani elementu, ZA kterym se nachazi hledany element
            Element_t *curr;
            for (curr = m_pHead; curr->pNext != nullptr &&
                                 curr->pNext->value > value; curr = curr->pNext)
                ;   // zamerne zastaveni

            // Pokud element "curr" neni posledni element ve fronte
            if (curr->pNext != nullptr && curr->pNext->value == value)
            {
                Element_t *temp = curr->pNext;
                curr->pNext = temp->pNext;
                delete temp;
                return true;
            }
        }
    }
    return false;
}

PriorityQueue::Element_t *PriorityQueue::Find(int value)
{
    // Hledani elementu ve fronte
    Element_t *element = nullptr;
    for (element = m_pHead; element != nullptr && 
                            element->value != value; element = element->pNext)
        ;       // zamerne zastaveni
    return element;
}

size_t PriorityQueue::Length()
{
    int length = 0;
    for (Element_t *temp = m_pHead; temp != nullptr; temp = temp->pNext)
        length++;

    return length;
}

PriorityQueue::Element_t *PriorityQueue::GetHead()
{
    return m_pHead;
}

/*** Konec souboru tdd_code.cpp ***/
