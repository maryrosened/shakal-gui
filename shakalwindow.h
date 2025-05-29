#ifndef SHAKALWINDOW_H
#define SHAKALWINDOW_H

#include <QMainWindow>
#include "ui_shakalwindow.h"
#include <filesystem>

QT_BEGIN_NAMESPACE
namespace Ui {
class ShakalWindow;
}
QT_END_NAMESPACE

class ShakalWindow : public QMainWindow
{
    Q_OBJECT

public:
    ShakalWindow(QWidget *parent = nullptr);
    ~ShakalWindow();

private slots:
    void onSliderValueChanged(int value);
    void onSecondSliderValueChanged(int value);
    void updateShakalImage();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    void applyShakal(size_t depth, const std::filesystem::path& sourcePath, std::filesystem::path& targetPath);

    Ui::ShakalWindow *ui;
    size_t currentShakalDepth = 1; // Храним текущую глубину
    size_t currentShakalShakalDepth = 1; // Храним текущую глубину
    std::filesystem::path m_currentImagePath;
    std::filesystem::path m_processedImagePath;
};
#endif // SHAKALWINDOW_H
