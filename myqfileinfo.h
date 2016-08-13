#ifndef MYQFILEINFO_H
#define MYQFILEINFO_H
#include <QFileInfo>

class myQFileInfo : public QFileInfo
{
public:
    myQFileInfo();

    int getGroupNo() const;
    void setGroupNo(int value);

private:
    int groupNo;
};

#endif // MYQFILEINFO_H
