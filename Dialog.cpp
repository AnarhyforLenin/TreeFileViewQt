
#include "dialog.h"
#include "ui_dialog.h"
Dialog::Dialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QString sPath = QDir::homePath();

    searchLineEdit = ui->lineEdit;
    listView = ui->listView;

    dirmodel = new QFileSystemModel(this);
    dirmodel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs |QDir::Files | QDir::Hidden);
    dirmodel->setRootPath(sPath);

    ui->treeView->setModel(dirmodel);

    filemodel = new QFileSystemModel(this);
    filemodel->setFilter(QDir::NoDotAndDotDot | QDir::Files | QDir::AllDirs |QDir::Hidden);
    filemodel->setRootPath(sPath);

    allfilemodel = new QFileSystemModel(this);
    allfilemodel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Hidden | QDir::Files);
    allfilemodel->setRootPath(sPath);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(searchLineEdit);
    layout->addWidget(ui->treeView);
    layout->addWidget(ui->listView);
    setLayout(layout);

    connect(searchLineEdit, &QLineEdit::textChanged, this, &Dialog::updateListViewVisibility);
    connect(searchLineEdit, &QLineEdit::textChanged, this, &Dialog::searchFiles);

    listView->hide();
}
Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_treeView_clicked(const QModelIndex &index)
{
    QString sPath = dirmodel->fileInfo(index).absoluteFilePath();
    filemodel->setRootPath(sPath);

    searchLineEdit->clear();
    listView->hide();
}


void Dialog::searchInDirectory(const QString &path, const QString &searchText, QStandardItemModel &resultsModel)
{
    QDir directory(path);
    QFileIconProvider iconProvider;
    if (!directory.exists())
        return;
    QStringList filesAndDirs = directory.entryList(QDir::NoDotAndDotDot | QDir::Files | QDir::AllDirs | QDir::Hidden);
    for (const QString &entry : filesAndDirs) {
        if (entry.contains(searchText, Qt::CaseInsensitive)) {
            QIcon fileIcon = iconProvider.icon(QFileInfo(directory.filePath(entry)));
            QStandardItem *item = new QStandardItem(fileIcon, entry);
            resultsModel.appendRow(item);
        }
        if (directory.cd(entry)) {
            searchInDirectory(directory.absolutePath(), searchText, resultsModel);
            directory.cdUp();
        }
    }
}

void Dialog::searchFiles()
{
    QString searchText = searchLineEdit->text();
    if (searchText.isEmpty()) {
        ui->listView->setModel(filemodel);
        return;
    }
    QStandardItemModel *listViewModel = new QStandardItemModel();
    QString rootPath = filemodel->rootPath();
    searchInDirectory(rootPath, searchText, *listViewModel);
    ui->listView->setModel(listViewModel);
}
void Dialog::updateListViewVisibility(const QString &text)
{
    if (text.isEmpty()) {
        ui->listView->hide();
    } else {
        ui->listView->show();
    }
}