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
    void updateShakalImage();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    Ui::ShakalWindow *ui;
    size_t currentShakalDepth = 1; // Храним текущую глубину
    std::filesystem::path m_currentImagePath;
};
#endif // SHAKALWINDOW_H
