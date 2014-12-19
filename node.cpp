#include "node.h"


Node::Node()
{
    this->content = 0;
    this->left = this->right = 0;
    this->frequency = 0;
    this-> code.clear();
}

Node::Node(int frequency, unsigned char content = 0)
{
    this->content = content;
    this->frequency = frequency;
    this->left = this->right = 0;
}


Node *Node::getLeft()
{
    return this->left;
}

Node *Node::getRight()
{
    return this->right;
}

int Node::getFrequency()
{
    return this->frequency;
}

unsigned char Node::getContent()
{
    return this->content;
}

bool Node::isLeaf()
{
    return !(this->right && this->left);
}

void Node::printCode()
{
    for (int i = 0; i < code.size(); i++)
    {
        std::cout << code.at(i);
    }

    std::cout << std::endl;
}


QByteArray Node::getCode()
{
    return code;
}

void Node::setLeft(Node *newLeft)
{
    left = newLeft;
}

void Node::setRight(Node *newRight)
{
    right = newRight;
}

void Node::setCode(QByteArray code)
{
    this->code.append(code);
}

void Node::setFrequency(int frequency)
{
    this->frequency = frequency;
}

void Node::setContent(unsigned char content)
{
    this->content = content;
}
