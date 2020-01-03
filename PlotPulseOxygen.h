#pragma once
/************************************************************************
* filename:	PlotPulseOxygen
* function:	��������
* author :	������
* datetime:	2019-04-28
* company:  ���̽ݿƼ��ɷ����޹�˾
/************************************************************************/

#include <QObject>

class QwtPlot;

class PlotPulseOxygen : public QObject
{
public:
    PlotPulseOxygen(QwtPlot *plot);
    ~PlotPulseOxygen();

    void SetParam(unsigned uSamplingRate , double dXAxisWidth, unsigned uPlotIntervalTime);

    void BuildPlot();

    void Start();
    void Stop();

    //���Ӳ�������
    void AddWaveData(QList<unsigned char> list);

    //������ʷ����
    void DrawHistoryData(quint64 ulStartTime, QVector<unsigned char> vctSPO2, QVector<unsigned char> vctPulse);

    void InitPlot();
    //��������
    void AddPlotGrid();

    //���ӱ��
    void AddPlotMarker();

    //��������ͼ��(��ʾ��������)
    void AddLegend();

    //��������ƽ��
	void AddPanner();

	// ���ӹ��ַŴ�/��С
	void AddMagnifier();

    //���Ӱ�������
    void AddZoomer();

	//����ʰȡ��
	void AddPicker();

    //���Ӳ�������
    void AddWaveCurve();


protected:
    virtual void timerEvent( QTimerEvent * );
    //���»���
    void UpdateCurve(qint64 elapsed);

private:
    QwtPlot              *m_pPlot;

	class PrivateData;
    PrivateData			 *data;
};