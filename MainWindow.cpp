#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "PlotBloodOxygenWave.h"
#include "MSamplingThread.h"
#include "PlotPulseOxygen.h"
#include <QDebug>
#include <QDateTime>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	m_pPlotMng = new PlotBloodOxygenWave(ui->qwtPlot);
    m_pPlotMngTwo = new PlotPulseOxygen(ui->qwtPlotTwo);

	m_thSampling.reset(new MSamplingThread(m_pPlotMng));
	m_thSampling->setInterval(1000);
	
    //设置参数
    on_btnConfirmModify_clicked();
    //构建绘制
	m_pPlotMng->BuildPlot();   
	m_pPlotMng->AddPanner();
	m_pPlotMng->AddMagnifier();
	ui->qwtPlot->enableAxis(QwtPlot::xBottom, true);

    m_pPlotMngTwo->BuildPlot();
}

MainWindow::~MainWindow()
{
    m_thSampling->stop();
    if(false == m_thSampling->wait(2000))
    {
        qDebug()<<"停止采样线程超时，将会强制终止！";
//        m_thSampling->terminate();
    }
	//delete m_thSampling;

    m_pPlotMng->Stop();

    delete ui;
}

void MainWindow::on_btnStart_clicked()
{
    if("start" == ui->btnStart->text()  )
    {
        ui->btnStart->setText("stop");
		m_pPlotMng->Reset();

		m_pPlotMng->SetMagnifierEnabled(false);
		m_pPlotMng->SetPannerEnabled(false);
        m_pPlotMng->Start();
        m_thSampling->start();
    }
    else
    {
        ui->btnStart->setText("start");
        m_pPlotMng->Stop();
        m_thSampling->stop();

		m_pPlotMng->SetMagnifierEnabled(true);
		m_pPlotMng->SetPannerEnabled(true);
    }
}

/*
//实例化图例
    QwtLegend *legend = new QwtLegend;
    //图例可以选择,Checkable
    legend->setDefaultItemMode( QwtLegendData::Checkable );
    //pwtPlot中插入图例
    ui->qwtPlot->insertLegend(legend, QwtPlot::LeftLegend );
    //连接槽，处理选择事件
    connect( legend, SIGNAL( checked( const QVariant &, bool, int ) ),
        SLOT( legendChecked( const QVariant &, bool ) ) );
//图例选择事件
void MainWindow::legendChecked( const QVariant &itemInfo, bool on )
{
   //获取曲线
   QwtPlotItem *plotItem = ui->qwtPlot->infoToItem( itemInfo );
   //根据图例选择状态，设置曲线隐藏还是显示
   if ( plotItem )
       plotItem->setVisible(on);
   //重要，下面这句没有不起作用
   ui->qwtPlot->replot();
}  */

void MainWindow::on_btnConfirmModify_clicked()
{
    unsigned uValue = 0;
    unsigned uSamplingRate = 0;
    QString qstrInterval;
    uSamplingRate = ui->leSamplingRate->text().toUInt();

    uValue =  1000 / uSamplingRate;
    qstrInterval += QString::number( uValue).append("ms / ");
    uValue = 1000 % uSamplingRate;
    qstrInterval += QString::number( uValue).append("ms");
    ui->leIntervalTime->setText(qstrInterval);

	m_thSampling->SetSamplingRate(uSamplingRate);
    m_pPlotMng->SetParam(uSamplingRate, ui->leXAxisWidth->text().toDouble(),  ui->lePlotIntervalTime->text().toUInt());
    m_pPlotMngTwo->SetParam(1, ui->leXAxisWidth->text().toDouble() * 60, 2000);
}

void MainWindow::on_btnDrawHistory_clicked()
{
	if ("stop" == ui->btnStart->text())
		on_btnStart_clicked();

    unsigned uSamplingRate = ui->leSamplingRate->text().toUInt();
    qint64    uDrawLength = ui->leDrawLength->text().toUInt();
    uDrawLength *= 3600 * uSamplingRate ;	//5H
    QVector<unsigned char> vctHistory;
    vctHistory.reserve(uDrawLength);
    for(qint64 i = 0; i < uDrawLength; i++)
    {
        vctHistory.append(rand() % 255);
    }

    QElapsedTimer   time;
    time.start();
    m_pPlotMng->DrawHistoryData(0, vctHistory);
    qDebug()<<QStringLiteral("绘制用时:%1ms").arg(QString::number(time.elapsed()));
}

void MainWindow::on_btnSecondDrawHistory_clicked()
{
    qint64    uDrawLength = ui->leSecondDrawLength->text().toDouble() * 3600;
    QVector<unsigned char> vctSPOHistory;
    vctSPOHistory.reserve(uDrawLength);
    QVector<unsigned char> vctPulseHistory;
    vctPulseHistory.reserve(uDrawLength);

    unsigned char uValue = 0;
    for(qint64 i = 0; i < uDrawLength; i++)
    {
       uValue = 70 + rand() % 10;
        vctSPOHistory.append(uValue);
        uValue = 60 + rand() % 10;
        vctPulseHistory.append(uValue);
    }

    QElapsedTimer   time;
    time.start();
    m_pPlotMngTwo->DrawHistoryData(QDateTime::currentMSecsSinceEpoch(), vctSPOHistory, vctPulseHistory);
    qDebug()<<QStringLiteral("绘制用时:%1ms").arg(QString::number(time.elapsed()));
}
