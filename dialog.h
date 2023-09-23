#ifndef DIALOG_H
#define DIALOG_H

#include <QWidget>
#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QListWidget>
#include <QList>
#include <QVBoxLayout>
#include <QFileSystemModel>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QFileIconProvider>

namespace Ui {
class Dialog;
}

class Dialog : public QWidget
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_treeView_clicked(const QModelIndex &index);
    void searchFiles();
    void updateListViewVisibility(const QString &text);

private:
    Ui::Dialog *ui;
    QFileSystemModel *dirmodel;
    QFileSystemModel *filemodel;
    QFileSystemModel *allfilemodel;
    QLineEdit *searchLineEdit;
    QListView *listView;

    void searchInDirectory(const QString &path, const QString &searchText, QStandardItemModel &resultsModel);
};

#endif // DIALOG_H
