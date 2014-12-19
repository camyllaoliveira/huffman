#ifndef TREE_H
#define TREE_H

#include "node.h"
#include <QList>
#include <QFile>
#include <QStack>
#include <QBitArray>

using namespace std;

class Tree
{
    QList<Node*>* list;
    Node* root;
    QString representation;
    QList<QByteArray> codes;
    QByteArray codeArray;
    char sizeTrash;

public:
    // Builder --------------------------------------
    Tree();

    // Gets -----------------------------------------
    Node* getRoot();
    QByteArray getCodification();
    QString getRepresentation() const;
    int getSizeTree();

    // Builders the tree -----------------------------
    void buildTree(int bytes[]);

    // Builders representation -----------------------
    void buildRepresentation(Node* base);

    // Encode ----------------------------------------
    void codification(Node* node,QString fileName);

    // Deco -----------------------------------------
    void decodification(QBitArray bits, int pos, Node* node);

    // Two firts bytes trash and tree's size --------
    QByteArray trashAndTreeSize();

    // rebuilder ------------------------------------
    void rebuilder(QByteArray rep, Node *node);

    // returns trash -------------------------------
    int getTrash();

    // ---------------------------------------------
    void clearCodeArray();

    // Sets -----------------------------------------
    void setTrash(QByteArray byte);
    void setRoot(Node* node);

    // Countleafs -----------------------------------
    unsigned int countLeaf(Node* node);

    // DELETE THIS -----------------------------------
    void printNo(Node *node, int level = 0);
    void printCodes();
    void printDeco();

private:

    // Utilitárias  ---------------
    void bubbleSort();
    int position(Node* node) const;
    void encodeHelper(Node *node, QByteArray code);


};

#endif // TREE_H
