#include "shakalwindow.h"
#include "./ui_shakalwindow.h"
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QLabel>
#include <QDebug>
#include "ImageDecoder.hpp"

std::filesystem::path inputImageForTest = "../tests/input.png";

ShakalWindow::ShakalWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ShakalWindow)
{
    setWindowTitle("Shakal Window — Drop Image Here");
    setAcceptDrops(true);
    ui->setupUi(this);

    ui->label->setText("Перетащите изображение сюда");
    ui->label->setMinimumSize(400, 300);
    // ui->label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->label->setScaledContents(true);

    // Где-то в конструкторе:
    connect(ui->horizontalSlider, &QSlider::valueChanged, this, &ShakalWindow::onSliderValueChanged);

    // Установим начальное значение (например, от 1 до 10)
    ui->horizontalSlider->setRange(1, 10);
    ui->horizontalSlider->setValue(currentShakalDepth);
}

void ShakalWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void ShakalWindow::onSliderValueChanged(int value) {
    currentShakalDepth = static_cast<size_t>(value);
    qDebug() << "Текущая глубина шакализации:" << currentShakalDepth;
    updateShakalImage();
}

void ShakalWindow::dropEvent(QDropEvent *event)
{
    const auto urls = event->mimeData()->urls();
    if (urls.isEmpty()) return;

    QString filePath = urls.first().toLocalFile();
    qDebug() << "Файл загружен:" << filePath;

    // Сохраняем путь, чтобы можно было обновлять шакализацию
    m_currentImagePath = filePath.toStdString();

    // Обновляем изображение с текущей глубиной
    updateShakalImage();

}

void ShakalWindow::updateShakalImage() {
    if (m_currentImagePath.empty()) {
        qDebug() << "Нет загруженного изображения для шакализации";
        return;
    }

    converter::ImageConverter tmp(m_currentImagePath);
    tmp.ShakalImage(currentShakalDepth);

    std::filesystem::path outputPath = "../shakal.png";
    tmp.SaveImage(outputPath.string());

    QImage resultImage(QString::fromStdString(outputPath.string()));
    if (resultImage.isNull()) {
        ui->label->setText("Ошибка: не удалось загрузить обработанное изображение");
        return;
    }

    QPixmap pixmap = QPixmap::fromImage(resultImage);
    ui->label->setPixmap(pixmap);
}

ShakalWindow::~ShakalWindow()
{
    delete ui;
}
