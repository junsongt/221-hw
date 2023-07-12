#include "chain.h"

#include <cmath>
#include <iostream>

#include "chain_given.cpp"

// PA1 functions

/**
 * Destroys the current Chain. This function should ensure that
 * memory does not leak on destruction of a chain.
 */
Chain::~Chain() {
    // Your code here -- hint: this one is a single line!
    clear();
}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
void Chain::insertBack(const Block& ndata) {
    // Your code here
    Chain::Node* newNode = new Node(ndata);

    newNode->prev = this->tail_->prev;
    newNode->next = this->tail_;
    this->tail_->prev->next = newNode;
    this->tail_->prev = newNode;

    length_ = length_ + 1;
}

void Chain::swapPtr(Node*& a, Node*& b) {
    Node* temp = a;
    a = b;
    b = temp;
}

/**
 * Swaps the two nodes at the indexes "node1" and "node2".
 * The indexes of the nodes are 1 based.
 * assumes i and j are valid (in {1,length_} inclusive)
 * 
 */
void Chain::swap(int i, int j) {
    // Your code here!
    if (i == j) {
        return;
    }
    if (i > j) {
        swap(j, i);
    } else {
        Node* ith = walk(this->head_, i);
        Node* jth = walk(ith, j-i);
        if (j - i == 1) {
            ith->prev->next = jth;
            jth->next->prev = ith;
            ith->next = jth->next;
            jth->prev = ith->prev;
            ith->prev = jth;
            jth->next = ith;
        } else {
            ith->prev->next = jth;
            ith->next->prev = jth;
            jth->prev->next = ith;
            jth->next->prev = ith;
            
            swapPtr(ith->prev, jth->prev);
            swapPtr(ith->next, jth->next);
        }
    }
    // Node* ith = walk(this->head_, i);
    // Node* jth = walk(this->head_, j);
    // if (i == j) {
    //     return;
    // }
    // if (j - i == 1) {
    //     ith->prev->next = jth;
    //     jth->next->prev = ith;
    //     ith->next = jth->next;
    //     jth->prev = ith->prev;
    //     ith->prev = jth;
    //     jth->next = ith;
    // } else {
    //     ith->prev->next = jth;
    //     ith->next->prev = jth;
    //     jth->prev->next = ith;
    //     jth->next->prev = ith;
    //        
    //     swapPtr(ith->prev, jth->prev);
    //     swapPtr(ith->next, jth->next);
    // }   
}

/**
 * Reverses the chain
 */
void Chain::reverse() {
    // Your code here!
    Node* curr = this->head_->next;
    while (curr->next != NULL) {
        Node* temp = curr->next;
        swapPtr(curr->prev, curr->next);
        curr = temp;
    }
    swapPtr(this->head_, this->tail_);
    this->head_->next = this->head_->prev;
    this->tail_->prev = this->tail_->next;
    this->tail_->next = NULL;
    this->head_->prev = NULL;


}

/*
* Modifies the current chain by "rotating" every k nodes by one position.
* In every k node sub-chain, remove the first node, and place it in the last 
* position of the sub-chain. If the last sub-chain has length less than k,
* then don't change it at all. 
* Some examples with the chain a b c d e:
*   k = 1: a b c d e
*   k = 2: b a d c e
*   k = 3: b c a d e
*   k = 4: b c d a e
*/
void Chain::rotate(int k) {
    // Your code here!
    for (int i = 0; i < (length_ / k); i++) {
        for (int j = 1; j <= k - 1; j++) {
            swap(j + i * k, j + i * k + 1);
        }
    }
}

/**
 * Destroys all dynamically allocated memory associated with the
 * current Chain class.
 */
void Chain::clear() {
    // Your code here!
    Node* curr = head_->next;
    while (curr->next != NULL) {
        Node* nextNode = curr->next;
        nextNode->prev = head_;
        head_->next = nextNode;
        delete curr;
        curr = nextNode;
    }
    delete head_;
    head_ = NULL;
    delete tail_;
    tail_ = NULL;

    // while (head_->next != NULL) {
    //     Node* temp = head_->next;
    //     delete head_;
    //     head_ = temp;
    //     head_->prev = NULL;
    // }
    // delete head_;
    // head_ = NULL;
    // tail_ = NULL;
}

/* makes the current object into a copy of the parameter:
 * All member variables should have the same value as
 * those of other, but the memory should be completely
 * independent. This function is used in both the copy
 * constructor and the assignment operator for Chains.
 */
void Chain::copy(Chain const& other) {
    // Your code here!
    this->length_ = 0;
    this->numCols_ = other.numCols_;
    this->numRows_ = other.numRows_;
    this->height_ = other.height_;
    this->width_ = other.width_;
    head_ = new Node();
    tail_ = new Node();
    head_->next = tail_;
    tail_->prev = head_;

    Node* currOther = other.head_->next;
    while (currOther->next != NULL) {
        insertBack(currOther->data);
        currOther = currOther->next;
    }       
}

/***********************************
 * swapColumns
 * parameters: 1 <= i <= numCols_
 *              1 <= j <= numCols_
 *
 * Swaps the positions of columns i and j
 * in the original grid of blocks by
 * moving nodes in the chain.
 *
 ***********************************/

void Chain::swapColumns(int i, int j) {
    // Your code here
    // for (int a = i; a <= i + numCols_ * (numRows_ - 1); a = a + numCols_) {
    //     swap(a, a + (j - i));
    // }
    for (int a = 0; a < numRows_; a++) {
        swap(i + a * numCols_, j + a * numCols_);
    }
}

/***********************************
 * swapRows
 * parameters: 1 <= i <= numRows_
 *              1 <= j <= numRows_
 *
 * Swaps the positions of rows i and j
 * in the original grid of blocks by
 * moving nodes in the chain.
 *
 ***********************************/
void Chain::swapRows(int i, int j) {
    // Your code here
    for (int c = 1; c <= numCols_; c++) {
        swap(c + (i-1)*numCols_, c + (j-1)*numCols_);
    }
}
