#include "imageeditor.h"
#include "ui_imageeditor.h"
#include <memory>
#include <opencv2/ximgproc.hpp>
#include <opencv2/ximgproc/seeds.hpp>
#include <QMouseEvent>

ImageEditor::ImageEditor(std::string imagefile, QWidget *parent)
    : QWidget(parent), ui(new Ui::ImageEditor) {
  ui->setupUi(this);
  this->image = cv::imread(imagefile);
  this->superpixel = this->image;
  ui->imageLabel->setText(QString::fromStdString(imagefile));
  ui->superPixelLabel->installEventFilter(this);
  reloadImage();
}

ImageEditor::~ImageEditor() { delete ui; }

void ImageEditor::reloadImage() {

  ui->imageLabel->setPixmap(fromMat(image));
  ui->superPixelLabel->setPixmap(fromMat(superpixel));
  ui->maskLabel->setPixmap(fromMat(mask));
  ui->segmentLabel->setPixmap(fromMat(segment));
}

bool ImageEditor::eventFilter( QObject* watched, QEvent* event ) {
    if ( watched != ui->superPixelLabel )
        return false;
    if ( event->type() != QEvent::MouseButtonPress )
        return false;
    const QMouseEvent* const me = static_cast<const QMouseEvent*>( event );
    //might want to check the buttons here
    const QPoint p = me->pos(); //...or ->globalPos();
    paintSuperPixel(p);
    return false;
}

void ImageEditor::paintSuperPixel(QPoint p)
{

    uint CLASS = this->segment.at<uchar>(p.y(), p.x(), 0);
    this->mask = this->segment.clone();
    for(int x = 0; x < this->mask.cols; ++x){
        for(int y = 0; y < this->mask.rows; ++y){
            this->mask.at<uchar>(y, x, 0) = (this->mask.at<uchar>(y, x, 0)==CLASS)?255:0;
        }
    }
    reloadImage();
}

void ImageEditor::on_num_superpixels_valueChanged(int value) {
  SuperPixel_Seg(this->image, this->segment);
  reloadImage();
}

// source:
// https://github.com/daijialun/Saliency/blob/e8b1c951c1d809012f66f9a512aa0ddfc6ea40ee/SuperPixel_Seg.cpp
int ImageEditor::SuperPixel_Seg(cv::Mat src, cv::Mat &matLabels) {
  int height = src.rows, width = src.cols;
  int num_superpixels = ui->num_superpixels->value();
  int num_level = 4;
  int num_iterations = 4;
  cv::Mat mask = src.clone(), result = src.clone();
  cv::Ptr<cv::ximgproc::SuperpixelSEEDS> seeds;
  seeds = cv::ximgproc::createSuperpixelSEEDS(width, height, src.channels(),
                                              num_superpixels, num_level);
  cv::Mat converted;
  cv::cvtColor(src, converted, cv::COLOR_RGB2HSV);
  seeds->iterate(src, num_iterations);
  int superpixels_num = 0;
  superpixels_num = seeds->getNumberOfSuperpixels();
  seeds->getLabels(matLabels);
  seeds->getLabelContourMask(mask, false);
  result.setTo(cv::Scalar(0, 0, 255), mask);
  this->mask = mask;
  superpixel = result;
  return superpixels_num;
}

QPixmap ImageEditor::fromMat(cv::Mat &img) {
  QImage image = QImage((uchar *)img.data, img.cols, img.rows, img.step,
                        QImage::Format_RGB888);
  QPixmap pixel = QPixmap::fromImage(image);
  return pixel;
}
