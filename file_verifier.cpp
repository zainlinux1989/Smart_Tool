#include "file_verifier.h"

file_verifier::file_verifier(QObject *parent) : QObject(parent)
{

}

void file_verifier::setFileName(const QString &filePath,int checkSumType)
{
    //QString n=name;
    //fileName=n.remove(0,7);         // remove "file://" part of path string
    QFile f(filePath);
    if(f.open(QFile::ReadOnly))
    {
        QCryptographicHash hash((QCryptographicHash::Algorithm)checkSumType);      // checkSumType value 0 to 10
        {
//                    QCryptographicHash::Md4	0	Generate an MD4 hash sum
//                    QCryptographicHash::Md5	1	Generate an MD5 hash sum
//                    QCryptographicHash::Sha1	2	Generate an SHA-1 hash sum
//                    QCryptographicHash::Sha224	3	Generate an SHA-224 hash sum (SHA-2). Introduced in Qt 5.0
//                    QCryptographicHash::Sha256	4	Generate an SHA-256 hash sum (SHA-2). Introduced in Qt 5.0
//                    QCryptographicHash::Sha384	5	Generate an SHA-384 hash sum (SHA-2). Introduced in Qt 5.0
//                    QCryptographicHash::Sha512	6	Generate an SHA-512 hash sum (SHA-2). Introduced in Qt 5.0
//                    QCryptographicHash::Sha3_224	7	Generate an SHA3-224 hash sum. Introduced in Qt 5.1
//                    QCryptographicHash::Sha3_256	8	Generate an SHA3-256 hash sum. Introduced in Qt 5.1
//                    QCryptographicHash::Sha3_384	9	Generate an SHA3-384 hash sum. Introduced in Qt 5.1
//                    QCryptographicHash::Sha3_512	10	Generate an SHA3-512 hash sum. Introduced in Qt 5.1
            if(hash.addData(&f))
            {
                checkSum=hash.result().toHex();
            }
        }
    }


}

QByteArray file_verifier::getCheckSum() const
{
    return checkSum;
}

