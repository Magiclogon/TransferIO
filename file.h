#ifndef FILE_H
#define FILE_H
#pragma once
#include <QString>

struct ServerFile {
    QString filePath;
    qint64 fileSize;
    qint16 numberDownloads;
};

#endif // FILE_H
