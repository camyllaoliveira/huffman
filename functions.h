#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <QFile>
#include <QDebug>
#include "tree.h"

void countBytes(QString fileName, int bytes[])
{

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "countBytes: The file cannot be read";
        return;
    }

    while(!file.atEnd())
    {
        QByteArray line = file.readLine(1024);
        for (int i = 0; i < line.size(); ++i){
            ++bytes[(unsigned char) line.at(i)];
        }
    }

    file.close();
}

void compressHelper(QString fileName, QString newFileName, Tree* tree)
{
    QFile file(newFileName);
    file.open(QIODevice::WriteOnly);

    // Name's size --------------------------------------
    char ch = 0;
    char aux = 0;
    int displace = 0;
    QByteArray tmp;
    tmp.append(fileName.size());

    for(int i = 0; i < tmp.size(); i++)
    {
        aux = tmp.at(i);
        ch |= (aux << (7 - displace));
        displace++;
        if (displace == 8)
        {
            displace = 0;
            tmp.append(ch);
            ch = 0;
        }
    }

    // Writes to new file ----------------------------------
    QByteArray toFile;

    // Trash and Tree's size -------------------------------
    toFile.append(tree->trashAndTreeSize());
    toFile.append(tmp);
    toFile.append(fileName);
    toFile.append(tree->getRepresentation());

    file.write(toFile);
    file.write(tree->getCodification());
    file.close();

}

void decompress(QString fileName, QString local = 0)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "decompress: The file cannot be decompress.";
        return;
    }

    Tree* tree = new Tree();

    // Trash and tree's size --------------------------------------
    QByteArray bytes;
    bytes = file.read(1);
    // trash ------------------------------------------------------
    tree->setTrash(bytes);

    // tree --------------------------------------------------------
    bytes.append(file.read(1));
    int sizetree = 0;
    char aux = bytes.at(0);
    char ch = 31;
    ch &= aux;
    sizetree += ch;
    aux = 0;
    aux = bytes.at(1);
    ch = 0;
    ch |= aux;
    sizetree += ch;

    // the name's file ---------------------------------------------
    bytes.append(file.read(1));
    int name = bytes.at(2);
    bytes.append(file.read(name));
    QString thename;
    for (int i = 3; i < 3 + name; i++)
    {
        thename.append(bytes.at(i));
    }

    // the representation -------------------------------------------
    bytes.clear();
    bytes.append(file.read(sizetree));
    for (int i = 0; i < bytes.size(); i++);

    tree->rebuilder(bytes,tree->getRoot());

    // decodification -------------------------------------------------
    bytes.clear();
    int read = file.pos();
    int atEnd = file.size();
    file.seek(read);
    int sizeCode = atEnd - read;
    bytes.append(file.read(sizeCode));

    QBitArray bits;
    bits.resize(bytes.count()*8);

    for(int i = 0; i < bytes.count(); ++i)
        for(int b = 0; b  < 8; ++b)
            bits.setBit(i*8+b, bytes.at(i)&(1<<(7-b)));

    int newsize = bits.size() - tree->getTrash();
    bits.resize(newsize);

    tree->clearCodeArray();
    tree->decodification(bits,0,tree->getRoot());
    file.close();

    // to file ------------------------------------------------------------
    bytes.clear();
    bytes.append(tree->getCodification());
//    if (local.isNull())
        QFile fileOut(thename);
//    else
//        local.append(thename);
//        QFile fileOut(local);
    if (!fileOut.open(QIODevice::WriteOnly)){
        qDebug() << "Decompress failed";
        return;
    }

    fileOut.write(bytes);
    fileOut.close();
    qDebug() << "Decompress sucessed";
}

void helpPrint()
{
    qDebug() <<"\t\t........ Help ........";
    qDebug() << "huffman [COMMANDS]\n";
    qDebug() << "-h or --help";
    qDebug() << ">> display help <<\n";
    qDebug() << "-c original_file.extension -o output_file.huff";
    qDebug() << ">> compresses the original file with the name of the output file <<\n";
    qDebug() << "-c original_file.extension";
    qDebug() << ">> compresses the original file and output file will have the same name <<\n";
    qDebug() << "file.huff";
    qDebug() << ">> decompresses the file in the local folder with the original name <<\n";
    qDebug() << "file.huff -d /home/user/destination";
    qDebug() << ">> Decompresses the file in the ""/home/user/destination"" with the original name <<\n";
}

void compress(QString fileName, QString fileOutputName)
{
    int bytes[256] = {0};
    countBytes(fileName,bytes);
    // Montagem da árvore ----------------------------------

    Tree* tree = new Tree();
    tree->buildTree(bytes);

    // Representação da árvore -----------------------------

    tree->buildRepresentation(tree->getRoot());

    // Codificação do arquivo  ----------------------------

    tree->codification(tree->getRoot(),fileName);

    // Arquivo codificado ---------------------------------
    compressHelper(fileName,fileOutputName,tree);

    qDebug() << "Compression sucessed";
}

#endif // FUNCTIONS_H
