#ifndef NODE_H
#define NODE_H

#include <QByteArray>
#include <QString>
#include <QPair>
#include <iostream>
#include <QDebug>

class Node {

    Node* left;
    Node* right;
    QByteArray code;
    int frequency;
    unsigned char content;


public:

    Node();
    Node(int frequency, unsigned char content);


    // Gets ---------------------------------------------------
    Node* getLeft();
    Node* getRight();
    QByteArray getCode();
    int getFrequency();
    unsigned char getContent();


    // Sets ---------------------------------------------------
    void setLeft(Node* newLeft);
    void setRight(Node* newRight);
    void setCode(QByteArray code);
    void setFrequency(int frequency);
    void setContent(unsigned char content);

    // Others --------------------------------------------------
    bool isLeaf();

    // Delete this --------------------------------------------
    void printCode();
};


#endif // NODE_H
