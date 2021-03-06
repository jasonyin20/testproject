#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QElapsedTimer>
#include <memory>

namespace Ui {
class MainWindow;
}

class PlotBloodOxygenWave;
class MSamplingThread;
class PlotPulseOxygen;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnStart_clicked();
    void on_btnConfirmModify_clicked();

	void on_btnDrawHistory_clicked();//��������֪ͨ

    void on_btnSecondDrawHistory_clicked();

private:
    Ui::MainWindow *ui;
    PlotBloodOxygenWave                        *m_pPlotMng;
    std::shared_ptr<MSamplingThread>	m_thSampling;

    PlotPulseOxygen                                    *m_pPlotMngTwo;
};

#endif // MAINWINDOW_H
