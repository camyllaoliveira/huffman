#include "tree.h"

Tree::Tree()
{
    root = new Node();
    list = new QList<Node*>();
    for(int i = 0; i < 256; ++i) {
        this->codes.append(QByteArray());
    }

}

Node *Tree::getRoot()
{
    return root;
}

QByteArray Tree::getCodification()
{
    return codeArray;
}

QString Tree::getRepresentation() const
{
    return representation;
}

int Tree::getSizeTree()
{
    return representation.size();
}

void Tree::buildTree(int bytes[])
{

    for(int i = 0; i < 256; i++){
        if(bytes[i]){
            Node* tmp = new Node(bytes[i],i);
            list->append(tmp);
        }
    }

    bubbleSort();

    while(list->size() > 1){
        Node* tmp = new Node();
        tmp->setFrequency(list->at(0)->getFrequency()+list->at(1)->getFrequency());
        tmp->setLeft(list->at(0));
        tmp->setRight(list->at(1));
        list->removeFirst();
        list->removeFirst();
        list->insert(position(tmp),tmp);
    }
    root = list->at(0);
}

void Tree::buildRepresentation(Node *base)
{
    if(base->isLeaf()){
        if(base->getContent() == '#'
                || base->getContent() == '{'){
            representation += '{';
        }
        representation += (char) base->getContent();
    } else {
        representation += '#';
        buildRepresentation(base->getLeft());
        buildRepresentation(base->getRight());
    }

}

void Tree::codification(Node *node, QString fileName)
{
    // Encode the nodes -------------------------------
    QByteArray code;
    encodeHelper(node,code);

    // Codes to codeArray -----------------------------
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "The file cannot be read.\nCompression failed!";
        return;
    }

    while(!file.atEnd())
    {
        unsigned char byte;
        QByteArray line = file.readLine(1024);
        for (int i = 0; i < line.size(); ++i)
        {
            byte = line.at(i);
            codeArray.append(codes[byte]);
        }
    }

    // Encode the file ---------------------------------
    code.clear();
    int displace = 0;
    unsigned char byteAux1 = 0, byteAux2 = 0;

    int trash = 0;
    if (codeArray.size() % 8)
        trash = 8 - (codeArray.size() % 8);
    sizeTrash = trash;

    for (int i = 0; i < codeArray.size(); ++i)
    {
        byteAux1 = (codeArray.at(i) << (7 - displace));
        byteAux2 |= byteAux1;
        ++displace;
        if (displace == 8)
        {
            code.append(byteAux2);
//            qDebug() << code.toHex();
            byteAux2 = 0;
            displace = 0;
        }
    }

//    qDebug() << displace << code.toHex();
    if (displace < 8)
        code.append(byteAux2);
//    qDebug() << displace << code.toHex();
    codeArray.clear();
    codeArray.append(code);

}

void Tree::decodification(QBitArray bits, int pos, Node *node)
{
    if (pos == bits.size())
        return;
    if(bits.at(pos) == false){
        node = node->getLeft();
    } else if(bits.at(pos) == true){
        node = node->getRight();
    }
    if (node->isLeaf()){
        codeArray.append(node->getContent());
        node = root;
    }
    pos++;
    decodification(bits,pos,node);

}

QByteArray Tree::trashAndTreeSize()
{
    QByteArray ret;

    int byte = (sizeTrash << 5);

    if (representation.size() > 255)
    {
        QByteArray tmp;
        int displace = 3;
        char aux;
        tmp.append(representation.size());
        for(int i = 0; i < tmp.size(); i++)
        {
            aux = tmp.at(i);
            byte |= (aux << (7 - displace));
            displace++;
            if (displace == 8)
            {
                ret.append(byte);
                displace = 0;
                byte = 0;
            }
        }
        if (displace < 8)
            ret.append(byte);

    } else {
        ret.clear();
        ret.append((unsigned char)byte).append(representation.size());
    }
    return ret;
}


void Tree::rebuilder(QByteArray rep, Node* node)
{
    QStack<Node*> stack;
    stack.push(node);
    for (int i = 0; i < rep.size(); i++)
    {
        Node* newNode;
        if (rep.at(i) == '#')
        {
            if (!stack.isEmpty())
                newNode = stack.pop();
            newNode->setLeft(new Node);
            newNode->setRight(new Node);
            stack.push(newNode->getRight());
            stack.push(newNode->getLeft());


        } else if (rep.at(i) == '{')
        {
            i++;
            if (!stack.isEmpty())
                newNode = stack.pop();
            newNode->setContent(rep.at(i));

        } else {

            if (!stack.isEmpty())
                newNode = stack.pop();
            newNode->setContent(rep.at(i));
        }
    }

}

int Tree::getTrash()
{
    int trash = sizeTrash;
    return trash;
}

void Tree::clearCodeArray()
{
    codeArray.clear();
}

void Tree::setTrash(QByteArray byte)
{
    char ch = 224; // 1110 0000
    char aux = 0;

    // "And" 3 firts bits -------------------

    aux = byte.at(0);
    ch &= aux;
    aux = 7;
    ch = (ch >> 5);
    ch &= aux;

    sizeTrash = ch;

}

void Tree::setRoot(Node *node)
{
    root = node;
}

unsigned int Tree::countLeaf(Node *node)
{
    if(node == 0)
    {
        return 0;
    }
    else if (node->getLeft() == 0 && node->getLeft() == 0)
    {
        return 1;
    } else {
        return countLeaf(node->getLeft())+countLeaf(node->getRight());
    }
}

void Tree::bubbleSort()
{
    for (int i = 0; i < list->size(); i++){
        for (int j = i+1; j < list->size(); ++j){
            if (list->at(i)->getFrequency() > list->at(j)->getFrequency()){
                list->swap(i,j);

            } else if ( list->at(i)->getFrequency() == list->at(j)->getFrequency()){

                if(list->at(i)->getContent() > list->at(j)->getContent()){
                    list->swap(i,j);
                }
            }

        }
    }
}

int Tree::position(Node *node) const
{
    int i;
    for(i = 0; i < list->size(); ++i){
        if(node->getFrequency() < list->at(i)->getFrequency()){
            break;
        }
    }
    return i;
}

void Tree::encodeHelper(Node *node, QByteArray code){
    if (node->isLeaf()){
        node->setCode(code);
        qDebug() << (char) node->getContent() << node->getCode().toHex();
        codes.replace((unsigned char) node->getContent(), node->getCode());
    } else {
        QByteArray leftCode = code;
        QByteArray rightCode = code;
        encodeHelper(node->getLeft(), leftCode.append((char) 0));
        encodeHelper(node->getRight(), rightCode.append(1));
    }
}

// Delete this -------------------------------------------------------

void Tree::printNo(Node *node, int level) {
    if(node != 0){
          printNo(node->getRight(), level + 1);
          cout << qPrintable(QString("\t").repeated(level)) << node->getContent() << "/" << node->getFrequency() << endl;
          printNo(node->getLeft(), level + 1);
      }
}

void Tree::printCodes()
{
    for (int i = 0; i < 255; i++)
    {
        qDebug() << "-> " << codes[i];
    }
}

void Tree::printDeco()
{
    for (int i = 0; i < codeArray.size(); i++){
        qDebug() << "CodeArray" << (unsigned char) codeArray.at(i);
    }
}

