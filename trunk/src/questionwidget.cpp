/***************************************************************************
*   Copyright (C) 2005 by Tim Sutton   *
*   tim@linfiniti.com   *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/

#include "questionwidget.h"

#include <QColor>
#include <QFont>
#include <QFontMetrics>
#include <QLinearGradient>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QPointF>
#include <QRgb>
#include <QtGui>
#include <QProgressBar>
#include <QTime>
#include <QLineEdit>
#include <QSqlQuery>
#include <QLineEdit>


  QuestionWidget::QuestionWidget(QWidget *parent)
: QWidget(parent), QuestionWidgetBase()
{
  setupUi(this);
  //connect(actionStartQuiz, SIGNAL(triggered()), this, SLOT(startQuiz()));
  //connect(actionStopQuiz, SIGNAL(triggered()), this, SLOT(stopQuiz()));
  //connect(actionResetQuizScore, SIGNAL(triggered()), this, SLOT(resetQuizScore()));
  setBackgroundRole(QPalette::Base);
  mQuestion="";
  mStatus=NONE;
  mpTimer = new QTimer(this);
  connect(mpTimer, SIGNAL(timeout()), this, SLOT(increaseProgress()));
  mScore=0; 
  mQuestionsAsked=0;
  //user only gets a point if question is answered before transp. threshold is crossed
  mTimeoutThreshold=80;
  progressQuestion->setMaximum(mTimeoutThreshold);
  progressQuestion->setValue(0);
  pbnQuiz->setVisible(false);
}
void QuestionWidget::increaseProgress()
{
  progressQuestion->setValue(progressQuestion->value()+1);
  //qDebug("Timeout : " + QString::number(progressQuestion->value()).toLocal8Bit() 
  //		+ " / " + QString::number(progressQuestion->maximum()).toLocal8Bit());
  if (progressQuestion->value()==progressQuestion->maximum())
  {
    failed();		
    //simulate a user enter key
    stopQuiz();
  }
}

void QuestionWidget::setQuestion(QString theQuestion)
{
  mQuestion = theQuestion;
  update();
}

QString QuestionWidget::question()
{
  return mQuestion;
}
void QuestionWidget::setAnswer(QString theAnswer)
{
  mAnswer = theAnswer;
}

QString QuestionWidget::answer()
{
  return mAnswer;
}

void QuestionWidget::paintEvent(QPaintEvent *)
{
  //set up a painter to draw on
  QPainter myPainter(this);
  myPainter.setRenderHint(QPainter::Antialiasing);

  //define a font and prepare for drawing
  QPainterPath myQuestionTextPath, myAnswerTextPath;
  QFont myFont("Arial", 80);
  QFontMetrics myMetrics=QFontMetrics(myFont);
  myFont.setStyleStrategy(QFont::ForceOutline);
  int myWordHeight = myMetrics.height();

  //set up a pen to draw with
  QPen myPen;
  myPen.setStyle(Qt::SolidLine);
  myPen.setBrush(QBrush(QColor(127, 127, 127,255))); //0=transp
  myPen.setWidth(3);
  myPainter.setPen(myPen);

  // Work out the metrics of the question and answer
  // also deal with uninitialised question by prompting user to 
  // press enter to start the quizz
  bool myPromptToStartFlag=false;
  int myQuestionWidth=0;
  const QString myPrompt = tr("Press enter to start");
  if (mQuestion.isEmpty())
  {
    myQuestionWidth = myMetrics.width(myPrompt);
    myPromptToStartFlag=true;
  }
  else
  {
    myQuestionWidth = myMetrics.width(mQuestion);
  }
  int myQuestionXStart = (lblQuestion->width()/2)-(myQuestionWidth/2);
  int myQuestionXEnd = (lblQuestion->width()/2)+(myQuestionWidth/2);
  int myAnswerWidth = myMetrics.width(mAnswer);
  int myAnswerXStart = (lblQuestion->width()/2)-(myAnswerWidth/2);
  int myAnswerXEnd = (lblQuestion->width()/2)+(myAnswerWidth/2);

  //draw the question - white with semi transparent fill
  myPainter.setBrush(QColor(255,255,255,127));
  myQuestionTextPath.addText(myQuestionXStart, myWordHeight, myFont, (myPromptToStartFlag) ? myPrompt : mQuestion);
  myPainter.drawPath(myQuestionTextPath);

  //dont do anything more if we are in prompt to start mode
  if (!myPromptToStartFlag)
  {
    //modify the pen a pen
    myPen.setCapStyle(Qt::RoundCap);
    myPen.setJoinStyle(Qt::RoundJoin);
    myPen.setStyle(Qt::SolidLine);
    //this is just interesting - shows how to paint lines using a pixmap
    //myPen.setBrush(QBrush(QRgb("#dfefcf01"),QPixmap(":/zino_logo.png")));

    if (mStatus==NONE || mStatus==PASSED)
    {
      QLinearGradient myGradient = greenGradient();
      myPainter.setBrush(myGradient);
    }
    else //mStatus==FAILED
    {
      QLinearGradient myGradient = redGradient();
      myPainter.setBrush(myGradient);
    }
    //draw a rounded rectangle to obscure the answer 
    //it will be as wide as the wider of question or answer
    myPen.setBrush(QBrush(QColor(255, 255, 255,255))); //0=transp
    QPointF myTLPoint, myBRPoint;
    if (myQuestionXStart < myAnswerXStart)
    {
      myTLPoint = QPointF(myQuestionXStart-35,(myWordHeight*2));
      myBRPoint = QPointF(myQuestionXEnd+35,(myWordHeight*3));
    }
    else
    {
      myTLPoint = QPointF(myAnswerXStart-35,(myWordHeight*2));
      myBRPoint = QPointF(myAnswerXEnd+35,(myWordHeight*3));
    }
    drawRoundRect(myTLPoint,myBRPoint,&myPainter);	


    //draw the answer if we timed out or got it correct
    if (mStatus==FAILED || mStatus==PASSED)
    {
      myPainter.setBrush(QColor(255,255,255,150));
      myPen.setBrush(QBrush(QColor(255,255,255,150))); //0=transp
      myPainter.setPen(myPen);
      myAnswerTextPath.addText(myAnswerXStart, (myWordHeight*2)+45, myFont, mAnswer);
      myPainter.drawPath(myAnswerTextPath);
    }
    //todo work out the highlight shape
    myPen.setStyle(Qt::NoPen);
    myPainter.setPen(myPen);
    QLinearGradient myGradient = highlightGradient();
    myPainter.setBrush(myGradient);
    drawHighlight(myTLPoint,myBRPoint,&myPainter);	
  }
}

QLinearGradient QuestionWidget::greenGradient()
{
  //define a gradient
  QLinearGradient myGradient = QLinearGradient(this->width()/2,0,this->width()/2,this->height());
  myGradient.setColorAt(0.0,QColor(48, 168, 5));
  myGradient.setColorAt(0.5,QColor(36, 122, 4));
  myGradient.setColorAt(1.0,QColor(21, 71, 2));
  return myGradient;
}

QLinearGradient QuestionWidget::redGradient()
{
  //define a gradient
  QLinearGradient myGradient = QLinearGradient(this->width()/2,0,this->width()/2,this->height());
  myGradient.setColorAt(0.0,QColor(242, 14, 25));
  myGradient.setColorAt(0.5,QColor(175, 29, 37));
  myGradient.setColorAt(1.0,QColor(114, 17, 22));
  return myGradient;
}

QLinearGradient QuestionWidget::highlightGradient()
{
  //define another gradient for the highlight
  QLinearGradient myGradient = QLinearGradient(this->width()/2,0,this->width()/2,this->height());
  myGradient.setColorAt(1.0,QColor(255, 255, 255, 50));
  myGradient.setColorAt(0.5,QColor(255, 255, 255, 100));
  myGradient.setColorAt(0.0,QColor(255, 255, 255, 150));
  return myGradient;
}

void QuestionWidget::drawRoundRect(QPointF theTopLeftPoint, QPointF theBottomRightPoint, QPainter * thePainter)
{

  const float SWEEPANGLE=90.0;
  float myAngle=0.0;
  const float WIDTH=50.0;
  const float HEIGHT=50.0;
  QPainterPath myRectanglePath;
  myRectanglePath.moveTo(theBottomRightPoint.x(),theTopLeftPoint.y());
  //top right corner
  myRectanglePath.arcTo(theBottomRightPoint.x()-WIDTH,theTopLeftPoint.y()-HEIGHT, WIDTH, HEIGHT, myAngle , SWEEPANGLE);
  myAngle+=90;
  //line to top left 
  myRectanglePath.lineTo(theTopLeftPoint.x()+WIDTH, theTopLeftPoint.y()-HEIGHT);
  //top left corner
  myRectanglePath.arcTo(theTopLeftPoint.x(), theTopLeftPoint.y()-HEIGHT, WIDTH, HEIGHT, myAngle , SWEEPANGLE);
  myAngle+=90;
  //line to bottom left
  myRectanglePath.lineTo(theTopLeftPoint.x(),theBottomRightPoint.y()-HEIGHT);
  //bottom left corner
  myRectanglePath.arcTo(theTopLeftPoint.x(),theBottomRightPoint.y()-HEIGHT, WIDTH, HEIGHT, myAngle , SWEEPANGLE);
  myAngle+=90;
  //line to bottom right
  myRectanglePath.lineTo(theBottomRightPoint.x()-WIDTH,theBottomRightPoint.y());
  //bottom right corner
  myRectanglePath.arcTo(theBottomRightPoint.x()-WIDTH,theBottomRightPoint.y()-HEIGHT, WIDTH, HEIGHT, myAngle, SWEEPANGLE);
  //close path back to original start pos
  myRectanglePath.closeSubpath();
  thePainter->drawPath(myRectanglePath);
}

void QuestionWidget::drawHighlight(QPointF theTopLeftPoint, QPointF theBottomRightPoint, QPainter * thePainter)
{
  // in the beginning do it all like a rounded rect
  const float SWEEPANGLE=90.0;
  float myAngle=0.0;
  const float WIDTH=50.0;
  const float HEIGHT=50.0;
  QPainterPath myHighlightPath;
  myHighlightPath.moveTo(theBottomRightPoint.x(),theTopLeftPoint.y());
  //top right corner
  myHighlightPath.arcTo(theBottomRightPoint.x()-WIDTH,theTopLeftPoint.y()-HEIGHT, WIDTH, HEIGHT, myAngle , SWEEPANGLE);
  myAngle+=90;
  //line to top left 
  myHighlightPath.lineTo(theTopLeftPoint.x()+WIDTH, theTopLeftPoint.y()-HEIGHT);
  //top left corner
  myHighlightPath.arcTo(theTopLeftPoint.x(), theTopLeftPoint.y()-HEIGHT, WIDTH, HEIGHT, myAngle , SWEEPANGLE);
  myAngle+=90;
  //line to two thirds way down on left
  myHighlightPath.lineTo(theTopLeftPoint.x(),theTopLeftPoint.y() + 
      ((theBottomRightPoint.y()-theTopLeftPoint.y())/3));
  // calculate the lower midpoint for the curve
  qreal myLowerMidPointX = theTopLeftPoint.x()+((theBottomRightPoint.x()-theTopLeftPoint.x())/2);
  qreal myLowerMidPointY = theTopLeftPoint.y()+((theBottomRightPoint.y()-theTopLeftPoint.y())/2);
  QPointF myLowerMidPoint(myLowerMidPointX,myLowerMidPointY);
  // calculate the upper midpoint for the curve
  qreal myUpperMidPointX = theTopLeftPoint.x()+((theBottomRightPoint.x()-theTopLeftPoint.x())/2);
  qreal myUpperMidPointY = theTopLeftPoint.y();
  QPointF myUpperMidPoint(myUpperMidPointX,myUpperMidPointY);
  // calculate the endpoint one third down on the right
  QPointF myEndPoint(theBottomRightPoint.x(),theTopLeftPoint.y() + 
      ((theBottomRightPoint.y()-theTopLeftPoint.y())/3));
  //draw a bezier curve throught the center point to the end point
  myHighlightPath.cubicTo(myUpperMidPoint,myLowerMidPoint,myEndPoint);
  //close path back to original start pos
  myHighlightPath.closeSubpath();
  thePainter->drawPath(myHighlightPath);
}

void QuestionWidget::failed()
{
  mStatus=FAILED;
  update();
}

void QuestionWidget::passed()
{
  mStatus=PASSED;
  update();
}
void QuestionWidget::reset()
{
  mStatus=NONE;
  update();
}
void QuestionWidget::on_pbnQuiz_clicked()
{
  //if the progress bar is at 0 or maximum, just go straight on to 
  //asking a question
  if (progressQuestion->value()==0 || progressQuestion->value()==progressQuestion->maximum())
  {
    createQuestion();
    return;
  }

  //test if the text in leAnwswer matches the question
  if (leAnswer->text()!=answer() )
  {
    leAnswer->setText("");
    return;
  }
  //see if the user deserves a point
  if (progressQuestion->value() < progressQuestion->maximum()) 
  {
    int myWordScore;
    bool myFlag = false;
    {
      QSqlQuery myQuery;
      mScore ++;
      //also keep track of the number of times a word has scored in the 
      //database so that we can ask the words user knows poorly most often
      myFlag = myQuery.exec("SELECT score from zino where lang1  = '" + question() + "'");
      //hideNotice();
      if (!myFlag ) 
      {
        //showNotice("A problem was encountered while getting the score for the question");
        return;
      }
      myQuery.first();
      myWordScore=myQuery.value(0).toInt();
      myWordScore++;
    }
    //now do a second query to update the score in the db
    QSqlQuery myQuery;
    QString mySQL="update zino set score = " + QString::number(myWordScore) +
      " where lang1  = '" + question() + "'";
    myFlag = myQuery.exec(mySQL);
    //hideNotice();
    if (!myFlag ) 
    {
      //showNotice("A problem was encountered while updating the score for the question\n"
        //  + mySQL);
      return;
    }
  }
  mpTimer->stop();
  progressQuestion->setValue(0);
  passed();
  //statusBar()->showMessage(tr("Correct! Press enter for next question")); 
}

void QuestionWidget::createQuestion()
{
  reset();
  QSqlQuery myQuery;
  bool myFlag = false;
  //create a new question
  //
  // SQLITE does not seem to support returning querysize which is required for
  // QSqlQuery.size() to work. So we make two queries, the first to count recs
  // and the second to select one of the recs at random.
  //
  if (question().isEmpty())
  {
    myFlag = myQuery.exec("SELECT count(*) from zino");
  }
  else
  {
    // <> clause is to make sure we dont ask the same rec as before
    myFlag = myQuery.exec("SELECT count(*) from zino where lang1  <> '" + 
        question() + "'");
  }
  //hideNotice();
  if (!myFlag ) 
  {
    //showNotice("A problem was encountered while generating the quizzme question");
    return;
  }
  myQuery.first();
  int myRecordCount=myQuery.value(0).toInt();
  if (myRecordCount<1)
  {
    //showNotice("Quizzme question cant be generated as you have no words in your wordlist");
    return;
  }
  //we will take our record from the lower half of the total records
  //where the half comprises only words with low scores
  myFlag = myQuery.exec("SELECT * from zino where lang1  <> '" + 
      question() + "' order by score, lang1 limit " + 
      QString::number(myRecordCount/2));
  //hideNotice();
  if (!myFlag ) 
  {
    //showNotice("A problem was encountered while generating the quizzme question");
    return;
  }
  //get a random record from the resultset
  srand(QTime::currentTime().msec());
  int myRand = rand();
  int mySelectedRec =  myRand%(myRecordCount/2);
  //qDebug("Random no: " + QString::number(mySelectedRec).toLocal8Bit());
  //qDebug("Rec Count: " + QString::number(myRecordCount).toLocal8Bit());
  myQuery.seek(mySelectedRec);
  QString myString = myQuery.value(1).toString();
  setQuestion(myString);
  myString = myQuery.value(2).toString();
  setAnswer(myString);
  pbnQuiz->setText(tr("Answer!"));
  leAnswer->setText("");
  mQuestionsAsked++;
  showScore();
  int myCharCount = question().count();
  //reset the progress bar
  progressQuestion->setValue(0);
  //speed of time is based on length of answer
  mpTimer->start(myCharCount*10);//50 ms
}
void QuestionWidget::startQuiz()
{
  reset();
  createQuestion();
}
void QuestionWidget::stopQuiz()
{
  mpTimer->stop();
}
void QuestionWidget::resetQuizScore()
{
  reset();
  mQuestionsAsked=1;
  mScore=0;
  showScore();
}

void QuestionWidget::showScore()
{
  //statusBar()->showMessage(tr("Score : ") + QString::number(mScore) 
  //    + "/" + QString::number(mQuestionsAsked));
}
