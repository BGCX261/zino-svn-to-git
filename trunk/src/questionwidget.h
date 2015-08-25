
#ifndef QUESTIONWIDGET_H
#define QUESTIONWIDGET_H

#include <QWidget>
#include <QLinearGradient>
#include <QPointF>
#include <QPainter>
#include <ui_questionwidgetbase.h>

class QuestionWidget : public QWidget, private Ui:: QuestionWidgetBase
{
  Q_OBJECT

  public:
    QuestionWidget(QWidget *parent = 0);
    QString question();
    QString answer();
    void passed();
    void failed();
    void reset();
    enum STATUS {NONE, PASSED, FAILED};

  public slots:
    void on_pbnQuiz_clicked();
    void setQuestion(QString theQuestion);
    void setAnswer(QString theAnswer);
    void showScore();
    void resetQuizScore();
    void startQuiz();
    void increaseProgress();
    void stopQuiz();
    void createQuestion();

  protected:
    void paintEvent(QPaintEvent *event);

  private:
    QLinearGradient greenGradient();
    QLinearGradient redGradient();
    QLinearGradient highlightGradient();
    void drawRoundRect(QPointF theTopLeftPoint, QPointF theBottomRightPoint, QPainter * thePainter);
    void drawHighlight(QPointF theTopLeftPoint, QPointF theBottomRightPoint, QPainter * thePainter);
    QString mQuestion;
    QString mAnswer;
    int mTransparency;
    STATUS mStatus;
    QTimer * mpTimer;
    int mScore;
    int mQuestionsAsked;
    int mTimeoutThreshold;
};

#endif
