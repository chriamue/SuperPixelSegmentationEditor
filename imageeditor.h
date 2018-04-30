#ifndef IMAGEEDITOR_H
#define IMAGEEDITOR_H

#include <QWidget>
#include <opencv2/opencv.hpp>

namespace Ui {
class ImageEditor;
}

class ImageEditor : public QWidget {
  Q_OBJECT

public:
  explicit ImageEditor(std::string imagefile, QWidget *parent = nullptr);
  ~ImageEditor();
  void reloadImage();
  int SuperPixel_Seg(cv::Mat src, cv::Mat &matLabels);
  QPixmap fromMat(cv::Mat &img);
  bool eventFilter( QObject* watched, QEvent* event );
  void paintSuperPixel(QPoint p);

private slots:
  void on_num_superpixels_valueChanged(int value);

private:
  Ui::ImageEditor *ui;
  cv::Mat image;
  cv::Mat superpixel;
  cv::Mat mask;
  cv::Mat segment;
};

#endif // IMAGEEDITOR_H
