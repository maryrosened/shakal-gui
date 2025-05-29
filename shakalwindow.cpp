#include "shakalwindow.h"
#include "./ui_shakalwindow.h"
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QLabel>
#include <QDebug>
#include "ImageDecoder.hpp"

ShakalWindow::ShakalWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ShakalWindow)
{
    setWindowTitle("Shakal Window — Drop Image Here");
    setAcceptDrops(true);
    ui->setupUi(this);

    ui->label->setText("Перетащите изображение сюда");
    ui->label->setMinimumSize(400, 300);
    ui->label->setScaledContents(true);

    // Где-то в конструкторе:
    connect(ui->horizontalSlider, &QSlider::valueChanged, this, &ShakalWindow::onSliderValueChanged);
    connect(ui->horizontalSlider_2, &QSlider::valueChanged, this, &ShakalWindow::onSecondSliderValueChanged);

    // Установим начальное значение (например, от 1 до 10)
    ui->horizontalSlider->setRange(1, 20);
    ui->horizontalSlider->setValue(currentShakalDepth);
    ui->horizontalSlider_2->setRange(1, 20);
    ui->horizontalSlider_2->setValue(currentShakalShakalDepth);
}

void ShakalWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void ShakalWindow::applyShakal(size_t depth, const std::filesystem::path &sourcePath, std::filesystem::path &targetPath)
{
    if (!std::filesystem::exists(sourcePath)) {
        qDebug() << "Файл не найден:" << QString::fromStdString(sourcePath.string());
        return;
    }

    converter::ImageConverter tmp(sourcePath);
    tmp.ShakalImage(depth);

    std::filesystem::path newFilePath = "../tests/";
    std::filesystem::create_directory(newFilePath);
    newFilePath /= "shakal.png";
    tmp.SaveImage(newFilePath.string());

    targetPath = newFilePath;

    updateShakalImage();
}

void ShakalWindow::onSliderValueChanged(int value)
{
    currentShakalDepth = static_cast<size_t>(value);
    applyShakal(currentShakalDepth, m_currentImagePath, m_processedImagePath);
}

void ShakalWindow::onSecondSliderValueChanged(int value)
{
    currentShakalShakalDepth = static_cast<size_t>(value);
    applyShakal(currentShakalShakalDepth, m_processedImagePath, m_processedImagePath);
}

void ShakalWindow::dropEvent(QDropEvent *event)
{
    const auto urls = event->mimeData()->urls();
    if (urls.isEmpty()) return;

    QString filePath = urls.first().toLocalFile();
    qDebug() << "Файл загружен:" << filePath;

    // Сохраняем путь, чтобы можно было обновлять шакализацию
    m_currentImagePath = filePath.toStdString();
    m_processedImagePath = m_currentImagePath;
    // Обновляем изображение с текущей глубиной
    updateShakalImage();

}

void ShakalWindow::updateShakalImage()
{
    QImage resultImage(QString::fromStdString(m_processedImagePath.string()));
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
