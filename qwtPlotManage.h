#pragma once
/************************************************************************
* filename:	PlotBloodOxygenWave
* function:	绘制血氧描述波
* author :	明巧文
* datetime:	2019-04-28
* company:  安碧捷科技股份有限公司
/************************************************************************/

#include <QObject>

class QwtPlot;

class qwtPlotManage : public QObject
{
    Q_OBJECT
public:
    qwtPlotManage(QwtPlot *plot);
    ~qwtPlotManage();

    /*************************************************************************
    * function: 设置初始化参数
    * param uSamplingRate: 采样率
    * param dXAxisWidth:     X轴显示宽度，单位为s
    * param uPlotIntervalTime: 绘制间隔时间，单位为ms
    * return:
    *************************************************************************/
    void SetParam(unsigned uSamplingRate , double dXAxisWidth, unsigned uPlotIntervalTime);

    void BuildPlot();

    void Start();
    void Stop();

    //添加波形数据
    void AddWaveData(QList<unsigned char> list);

    //绘制历史数据
    void DrawHistoryData(quint64 ulStartTime, QVector<unsigned char> list);

    void InitPlot();
    //添加网格
    void AddPlotGrid();

    //添加标记
    void AddPlotMarker();

    //添加曲线图例(显示曲线名称)
    void AddLegend();

    // 添加滚轮放大/缩小
    void AddMagnifier();

    //添加左右平移
    void AddPanner();

    //添加缩放
    void AddZoomer();

    //添加波形曲线
    void AddWaveCurve();


protected:
    virtual void timerEvent( QTimerEvent * );
    //更新画布
    void UpdateCurve(qint64 elapsed);

private slots:
    void slotCanvasMoved(int dx, int dy);

private:
    QwtPlot              *m_pPlot;

	class PrivateData;
    PrivateData			 *data;
};
