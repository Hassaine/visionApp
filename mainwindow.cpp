#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "visionFunction.h"
#include "gameLogique.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    ui->graphicsView->setScene(new QGraphicsScene(this));
    ui->graphicsView->scene()->addItem(&pixmap);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startBtn_pressed()
{
    using namespace cv;



    //load all necessaire images for game logic

    int mouseInitX=0;
    int mouseInitY=550;

    int catInitPosition[2][2]={{139,320},{339,449}};

    int chesseInitPosition[3][2]={{309,86},{441,297},{39,243}};
    bool cheeseFounded [3]= {false,false,false};
    bool catFounded[2] = {false,false};
    bool win=false;


    //the score of the partie
    int score=0;


    //load all neccaissare image for the game
    Mat originalGrid = imread("C:\\Users\\HP\\Documents\\projetVision\\mouse\\house.jpg",CV_LOAD_IMAGE_COLOR);
    Mat grid = originalGrid.clone();
    Mat mouse =  imread("C:\\Users\\HP\\Documents\\projetVision\\mouse\\mouse.png",CV_LOAD_IMAGE_COLOR);
    Mat cheese =  imread("C:\\Users\\HP\\Documents\\projetVision\\mouse\\cheese.png",CV_LOAD_IMAGE_COLOR);
    Mat cat =  imread("C:\\Users\\HP\\Documents\\projetVision\\mouse\\cat.jpg",CV_LOAD_IMAGE_COLOR);



    //drow the icons on the grid
    mouse.copyTo(grid(cv::Rect(mouseInitX,mouseInitY,mouse.cols, mouse.rows)));
    for (int i = 0; i < 3; ++i) {
            cheese.copyTo(grid(cv::Rect(chesseInitPosition[i][0],chesseInitPosition[i][1],cheese.cols, cheese.rows)));
    }
    for (int i = 0; i < 2; ++i) {
            cat.copyTo(grid(cv::Rect(catInitPosition[i][0],catInitPosition[i][1],cat.cols, cat.rows)));
    }








    if(video.isOpened())
    {
        ui->startBtn->setText("Start");
        video.release();
        return;
    }

    bool isCamera;
    int cameraIndex = ui->videoEdit->text().toInt(&isCamera);
    if(isCamera)
    {
        if(!video.open(cameraIndex))
        {
            QMessageBox::critical(this,
                                  "Camera Error",
                                  "Make sure you entered a correct camera index,"
                                  "<br>or that the camera is not being accessed by another program!");
            return;
        }
    }
    else
    {
        if(!video.open(ui->videoEdit->text().trimmed().toStdString()))
        {
            QMessageBox::critical(this,
                                  "Video Error",
                                  "Make sure you entered a correct and supported video file path,"
                                  "<br>or a correct RTSP feed URL!");
            return;
        }
    }


    ui->startBtn->setText("Stop");

    //declare all the Mat object of the opencv processing
    Mat frame;
    Mat threshold1;
    Mat threshold2;
    Mat lower_red_hue_range;
    Mat upper_red_hue_range;
    Mat HSV;


    //fix coulour HSV boundries

    Scalar YellowLowBond = Scalar(20, 100, 100);
    Scalar YellowUpperBond = Scalar(30, 255, 255);


    Scalar BlueLowBond = Scalar(100, 150, 0);
    Scalar BlueUpperBond = Scalar(140, 255, 255);


    Scalar RedLowBond_1 =   Scalar(0, 100, 100);
    Scalar RedUpperBond_1 = Scalar(10, 255, 255);



    Scalar RedLowBond_2 = Scalar(160, 100, 100);
    Scalar  RedUpperBond_2 = Scalar(179, 255, 255);



    //use morphological Operator
    bool useMorphOps = true;


    //use convolution Operator
    bool usegaussFilter = true;
    int x1 = 0, y1 = 0;
    int x2 = 0, y2 = 0;


    video.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
    video.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);

    while(video.isOpened())
    {


        video >> frame;

        if(!frame.empty())
        {

            //trnasform the image to HSV
            cvtColor(frame, HSV, COLOR_BGR2HSV);


            //creat all threasHold images with zeros
            threshold1 = Mat(HSV.size(), 0, Scalar(0));
            threshold2 = Mat(HSV.size(), 0, Scalar(0));


            lower_red_hue_range = Mat(HSV.size(), 0, Scalar(0));
            upper_red_hue_range = Mat(HSV.size(), 0, Scalar(0));



            //detecting the color of the start

            if(ui->lineEdit_2->text().endsWith("blue")){
                for (int i = 0; i < HSV.rows; i++){

                    for (int j = 0; j < HSV.cols; j++){
                        Vec3b hsv = HSV.at<Vec3b>(i, j);
                        uchar hue = hsv.val[0];
                        uchar sat = hsv.val[1];
                        uchar value = hsv.val[2];
                        if (hue >= BlueLowBond.val[0] && hue <= BlueUpperBond.val[0] && sat >= BlueLowBond.val[1] && sat <= BlueUpperBond.val[1] && value >= BlueLowBond.val[2] && value <= BlueUpperBond.val[2]){
                            threshold1.at<uchar>(i, j) = 255;
                        }

                    }
                }

            }
            if(ui->lineEdit_2->text().endsWith("yellow")){
                for (int i = 0; i < HSV.rows; i++){

                    for (int j = 0; j < HSV.cols; j++){
                        Vec3b hsv = HSV.at<Vec3b>(i, j);
                        uchar hue = hsv.val[0];
                        uchar sat = hsv.val[1];
                        uchar value = hsv.val[2];
                        if (hue >= YellowLowBond.val[0] && hue <= YellowUpperBond.val[0] && sat >= YellowLowBond.val[1] && sat <= YellowUpperBond.val[1] && value >= YellowLowBond.val[2] && value <= YellowUpperBond.val[2]){
                            threshold1.at<uchar>(i, j) = 255;
                        }

                    }
                }
            }
            if(ui->lineEdit_2->text().endsWith("red")){

                for (int i = 0; i < HSV.rows; i++){

                    for (int j = 0; j < HSV.cols; j++){
                        Vec3b hsv = HSV.at<Vec3b>(i, j);
                        uchar hue = hsv.val[0];
                        uchar sat = hsv.val[1];
                        uchar value = hsv.val[2];
                        if (hue >= RedLowBond_1.val[0] && hue <= RedUpperBond_1.val[0] && sat >= RedLowBond_1.val[1] && sat <= RedUpperBond_1.val[1] && value >= RedLowBond_1.val[2] && value <= RedUpperBond_1.val[2]){
                            lower_red_hue_range.at<uchar>(i, j) = 255;
                        }

                    }
                }
                for (int i = 0; i < HSV.rows; i++){

                    for (int j = 0; j < HSV.cols; j++){
                        Vec3b hsv = HSV.at<Vec3b>(i, j);
                        uchar hue = hsv.val[0];
                        uchar sat = hsv.val[1];
                        uchar value = hsv.val[2];
                        if (hue >= RedLowBond_2.val[0] && hue <= RedUpperBond_2.val[0] && sat >= RedLowBond_2.val[1] && sat <= RedUpperBond_2.val[1] && value >= RedLowBond_2.val[2] && value <= RedUpperBond_2.val[2]){
                            upper_red_hue_range.at<uchar>(i, j) = 255;
                        }

                    }
                }
                addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, threshold1);
            }


            //detecting the color of the end
            if(ui->lineEdit->text().endsWith("blue")){
                for (int i = 0; i < HSV.rows; i++){

                    for (int j = 0; j < HSV.cols; j++){
                        Vec3b hsv = HSV.at<Vec3b>(i, j);
                        uchar hue = hsv.val[0];
                        uchar sat = hsv.val[1];
                        uchar value = hsv.val[2];
                        if (hue >= BlueLowBond.val[0] && hue <= BlueUpperBond.val[0] && sat >= BlueLowBond.val[1] && sat <= BlueUpperBond.val[1] && value >= BlueLowBond.val[2] && value <= BlueUpperBond.val[2]){
                            threshold2.at<uchar>(i, j) = 255;
                        }

                    }
                }

            }
            if(ui->lineEdit->text().endsWith("yellow")){

                    for (int i = 0; i < HSV.rows; i++){

                        for (int j = 0; j < HSV.cols; j++){
                            Vec3b hsv = HSV.at<Vec3b>(i, j);
                            uchar hue = hsv.val[0];
                            uchar sat = hsv.val[1];
                            uchar value = hsv.val[2];
                            if (hue >= YellowLowBond.val[0] && hue <= YellowUpperBond.val[0] && sat >= YellowLowBond.val[1] && sat <= YellowUpperBond.val[1] && value >= YellowLowBond.val[2] && value <= YellowUpperBond.val[2]){
                                threshold2.at<uchar>(i, j) = 255;
                            }

                        }
                    }
            }
            if(ui->lineEdit->text().endsWith("red")){

                for (int i = 0; i < HSV.rows; i++){

                    for (int j = 0; j < HSV.cols; j++){
                        Vec3b hsv = HSV.at<Vec3b>(i, j);
                        uchar hue = hsv.val[0];
                        uchar sat = hsv.val[1];
                        uchar value = hsv.val[2];
                        if (hue >= RedLowBond_1.val[0] && hue <= RedUpperBond_1.val[0] && sat >= RedLowBond_1.val[1] && sat <= RedUpperBond_1.val[1] && value >= RedLowBond_1.val[2] && value <= RedUpperBond_1.val[2]){
                            lower_red_hue_range.at<uchar>(i, j) = 255;
                        }

                    }
                }
                for (int i = 0; i < HSV.rows; i++){

                    for (int j = 0; j < HSV.cols; j++){
                        Vec3b hsv = HSV.at<Vec3b>(i, j);
                        uchar hue = hsv.val[0];
                        uchar sat = hsv.val[1];
                        uchar value = hsv.val[2];
                        if (hue >= RedLowBond_2.val[0] && hue <= RedUpperBond_2.val[0] && sat >= RedLowBond_2.val[1] && sat <= RedUpperBond_2.val[1] && value >= RedLowBond_2.val[2] && value <= RedUpperBond_2.val[2]){
                            upper_red_hue_range.at<uchar>(i, j) = 255;
                        }

                    }
                }
                addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, threshold2);
            }



            if (usegaussFilter){

                        GaussianBlur(threshold2, threshold2, cv::Size(9, 9), 2, 2);
                        GaussianBlur(threshold1, threshold1, cv::Size(9, 9), 2, 2);
                    }
             if (useMorphOps)
                    {

                        morphOps(threshold1);
                        morphOps(threshold1);
                        morphOps(threshold2);
                        morphOps(threshold2);
                    }

             // we need to track both color in order to calculate the angle and move the dogs
             if (trackFilteredObject(x1, y1, threshold1, frame) && trackFilteredObject(x2, y2, threshold2, frame))
                       {
                            //draw the line between (x1, y1) and (x2, y2)
                            arrowedLine(frame, Point(x1, y1), Point(x2, y2), Scalar(0, 255, 0), 7);
                            double angle = -( atan2(y2 - y1, x2 - x1)* (180.0 / 3.141592653589793238463));

                             string str=to_string(angle);
                             QString qstr =QString::fromStdString("0");

                             // top left
                             if(angle >= 30.0 and angle<=70 ){


                                 if(  mouseInitX-1 >= 0 && mouseInitY-1>=0){

                                      mouseInitX-=1;
                                      mouseInitY-=1;
                                 }




                                 qstr= QString::fromStdString("top left");

                             }
                             //left
                            if(angle >= -15.0 and angle<=15 ){

                                if(  mouseInitX-1 >= 0 ){

                                     mouseInitX-=1;

                                }




                                qstr= QString::fromStdString("left");

                            }
                            //right
                            if(angle <= -170.0 or angle>=170 ){

                                if( mouseInitX+mouse.cols+1 <= grid.cols ){
                                     mouseInitX+=1;

                                }





                                qstr= QString::fromStdString("right");

                            }
                            // top right
                            if(angle >= 110.0 and angle<=150 ){
                                if( mouseInitX+mouse.cols+1 <= grid.cols && mouseInitY-1>=0){
                                     mouseInitX+=1;
                                     mouseInitY-=1;


                                }





                                qstr= QString::fromStdString("top right");


                            }
                            //top
                            if(angle >= 80.0 and angle<=100 ){


                                if(  mouseInitY-1 >= 0){

                                     mouseInitY-=1;

                                }





                                qstr= QString::fromStdString("top");

                            }
                            //down
                            if(angle >= -100.0 and angle<=-80){

                                if(  mouseInitY+mouse.rows+1<=grid.rows){

                                     mouseInitY+=1;


                                }



                                qstr= QString::fromStdString("down");

                            }

                            //down right
                            if(angle >= -150.0 and angle <= -110){

                                if(  mouseInitY+mouse.rows+1<=grid.rows  && mouseInitX+mouse.cols+1 <= grid.cols){

                                     mouseInitY+=1;
                                     mouseInitX+=1;


                                }


                                qstr= QString::fromStdString("down right");

                            }

                            //down left
                            if(angle >= -70.0 and angle <= -30){

                                if(  mouseInitY+mouse.rows+1 <= grid.rows  && mouseInitX-1 >= 0){

                                     mouseInitY+=1;
                                     mouseInitX-=1;





                                }


                                qstr= QString::fromStdString("down left");

                            }


                            ui->videoEdit->setText(qstr);

                             //printf("the angel betwin the tow point is: %f\n", angle);

                         }



            //refresh the screen with the new position of the mouse
             refreshScreen(originalGrid,grid,mouse,cheese,cat,catInitPosition,chesseInitPosition,mouseInitX,mouseInitY);

             // count the score of the game
             Point gravityCenterMouse = GravityCenter(mouseInitX,mouseInitY, mouse.rows,mouse.cols);
             for (int i = 0; i < 3; ++i) {
                     //game logique
                      Point gravityCenterCheese =   GravityCenter(chesseInitPosition[i][0],chesseInitPosition[i][1], cheese.rows,cheese.cols);
                      float distance = distance2p(gravityCenterMouse,gravityCenterCheese);
                      if(distance <=30.0 && !cheeseFounded[i]){
                          cheeseFounded[i]=true;
                          score+=100;
                      }

             }

                 // discount the score of the game if we go around a cat
             for (int i = 0; i < 2; ++i) {
                     //game logique
                      Point gravityCenterCat =   GravityCenter(catInitPosition[i][0],catInitPosition[i][1], cat.rows,cat.cols);
                      float distance = distance2p(gravityCenterMouse,gravityCenterCat);
                      if(distance <=30.0 && !catFounded[i]){
                          catFounded[i]=true;
                          score-=100;
                      }

             }
             string str=to_string(score);
             QString qstr =QString::fromStdString(str);
             ui->wining->setText(qstr);


//           QImage qimg(frame.data,
//                              frame.cols,
//                              frame.rows,
//                              frame.step,
//                              QImage::Format_RGB888);


                        QImage qimg(grid.data,
                                    grid.cols,
                                    grid.rows,
                                    grid.step,
                                    QImage::Format_RGB888);

                  pixmap.setPixmap( QPixmap::fromImage(qimg.rgbSwapped()) );
                  ui->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);



        }
        qApp->processEvents();
       if(win){
            break;
       }
    }

    ui->startBtn->setText("new game");
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(video.isOpened())
    {
        QMessageBox::warning(this,
                             "Warning",
                             "Stop the video before closing the application!");
        event->ignore();
    }
    else
    {
        event->accept();
    }
}
