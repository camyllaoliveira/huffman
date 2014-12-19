#include "functions.h"

int main(int argc, char *argv[])
{
    // Compresses nameFile and OutputFile -------------------------
    if (argc == 5)
    {
        if (!strcmp(argv[1],"-c"))
        {
            if (!strcmp(argv[3],"-o")){

                    QString nameFile;
                    nameFile.append(argv[2]);
                    QString nameFileOut;
                    nameFileOut.append(argv[4]);

                    if (nameFileOut.endsWith(".huff"))
                    {
                        compress(nameFile,nameFileOut);

                    } else {
                        qDebug() << "The extension of output_file should be "".huff""";
                    }

            } else {
                qDebug() << "Command" << argv[3] << "not found";
                helpPrint();
            }
        } else {
            qDebug() << "Command" << argv[1] << "not found";
            helpPrint();
        }
    }

    // Compresses nameFile, outputFile.huff -------------------------
    if (argc == 3)
    {
        if(strcmp(argv[1],"-c") == 0){
            QString nameFile;
            nameFile.append(argv[2]);
            QString nameFileOut;
            nameFileOut.append(nameFile);
            int size = nameFileOut.size();
            nameFileOut.remove(size-3,3);
            nameFileOut.append("huff");
            compress(nameFile,nameFileOut);

        } else {
            qDebug() << "Command" << argv[1] << "not found";
            helpPrint();
        }

    }

    if (argc == 2){
        QString fileName;
        fileName.append(argv[1]);
        decompress(fileName);
    }

    if (argc == 4){
        if(strcmp(argv[2],"-d") == 0){
            QString local;
            local.append(argv[3]).append('/');
            QString fileName;
            fileName.append(argv[2]);
            decompress(fileName,local);
        }
    }



    return 0;
}
