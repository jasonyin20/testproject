#pragma once

#include <qwt_plot.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_canvas.h>
#include <qwt_painter.h>
#include <qwt_plot_picker.h>
#include <QTime>

class TimeScaleDraw: public QwtScaleDraw
{
public:
    virtual QwtText label( double v ) const
    {
//        v /= 1000;
//        return QString("%1s").arg(QString::number(v , 'f', 0));
        qint64 lCount = v;
//        QTime time(0,0);
//        time = time.addMSecs(iCount);
        QDateTime dt = QDateTime::fromMSecsSinceEpoch(lCount);
        return dt.toString("HH:mm:ss");
    }
};

class MCanvas: public QwtPlotCanvas
{
public:
    MCanvas( QwtPlot *plot = nullptr ):
        QwtPlotCanvas( plot )
    {
        // The backing store is important, when working with widget
        // overlays ( f.e rubberbands for zooming ).
        // Here we don't have them and the internal
        // backing store of QWidget is good enough.

        setPaintAttribute( QwtPlotCanvas::BackingStore, false );
//        setBorderRadius( 10 );

        if ( QwtPainter::isX11GraphicsSystem() )
        {
#if QT_VERSION < 0x050000
            // Even if not liked by the Qt development, Qt::WA_PaintOutsidePaintEvent
            // works on X11. This has a nice effect on the performance.

            setAttribute( Qt::WA_PaintOutsidePaintEvent, true );
#endif

            // Disabling the backing store of Qt improves the performance
            // for the direct painter even more, but the canvas becomes
            // a native window of the window system, receiving paint events
            // for resize and expose operations. Those might be expensive
            // when there are many points and the backing store of
            // the canvas is disabled. So in this application
            // we better don't disable both backing stores.

            if ( testPaintAttribute( QwtPlotCanvas::BackingStore ) )
            {
                setAttribute( Qt::WA_PaintOnScreen, true );
                setAttribute( Qt::WA_NoSystemBackground, true );
            }
        }

        setupPalette();
    }

private:
    void setupPalette()
    {
        QPalette pal = palette();

#if QT_VERSION >= 0x040400
        QLinearGradient gradient;
        gradient.setCoordinateMode( QGradient::StretchToDeviceMode );
        gradient.setColorAt( 0.0, QColor( 0, 49, 110 ) );
        gradient.setColorAt( 1.0, QColor( 0, 87, 174 ) );

        pal.setBrush( QPalette::Window, QBrush( gradient ) );
#else
        pal.setBrush( QPalette::Window, QBrush( color ) );
#endif

        // QPalette::WindowText is used for the curve color
        pal.setColor( QPalette::WindowText, Qt::green );

        setPalette( pal );
    }
};

class MPlotPicker : public QwtPlotPicker
{
public:
	explicit MPlotPicker(QWidget *canvas) : 
		QwtPlotPicker(canvas){}
	explicit MPlotPicker(int xAxis, int yAxis, QWidget *parent) : 
		QwtPlotPicker(xAxis, yAxis, parent){}
	explicit MPlotPicker(int xAxis, int yAxis,
		RubberBand rubberBand, DisplayMode trackerMode, QWidget *parent):
		QwtPlotPicker(xAxis, yAxis, rubberBand, trackerMode, parent) {}

protected:
	virtual QwtText trackerTextF(const QPointF &pos) const {
		QwtText trackerText;
		QString text;
		QDateTime dt;
		QColor bkColor;
		switch (rubberBand())
		{
		case HLineRubberBand:
			text.sprintf("%.4f", pos.y());
			break;
		case VLineRubberBand:
            dt = QDateTime::fromMSecsSinceEpoch(pos.x());
			text.append(dt.toString("HH:mm:ss zzz"));
			//text.append(dt.toString("HH:mm:ss zzz, ")).append(QString::number(pos.y(), 'f', 4));
			bkColor = trackerPen().color();
			bkColor.setAlpha(60);
			trackerText.setBackgroundBrush(bkColor);
			//trackerText.setBackgroundBrush(QColor("#acc4ef"));
			trackerText.setBorderRadius(10);
			break;
		default:
			text.sprintf("%.4f, %.4f", pos.x(), pos.y());
		}

		trackerText.setText(text);
		return trackerText;
	}
	QRect trackerRect(const QFont& font) const {
		QRect r = QwtPlotPicker::trackerRect(font);
		r += QMargins(6, 0, 6, 0);
		return r;
	}
};
