
#include <vector>
#include <stdio.h>
#include <string>
#include <iostream>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iterator>
#include <cstdlib>
#include <math.h>

#define WIN_W                           1200
#define WIN_H                           800

#define ORIGINE_BCK_X                   10
#define ORIGINE_BCK_Y                   10

#define ORIGINE_SMILE_X                 (190)
#define ORIGINE_SMILE_Y                 (20)

#define LEFT_SMILE_X_B                  (106)
#define TOP_SMILE_Y_B                   (20)

#define LEFT_SMILE_X_I                  (190)
#define TOP_SMILE_Y_I                   (20)

#define LEFT_SMILE_X_E                  (358)
#define TOP_SMILE_Y_E                   (20)

#define L_SMILE                         38

#define L_FIGURE                        19
#define H_FIGURE                        35

#define WIDTH_BCK_H                     450
#define HEIGHT_BCK_V                    450

#define ORIGINE_CELLS_X_B               (18)
#define ORIGINE_CELLS_Y_B               (80)

#define ORIGINE_CELLS_X_I               (18)
#define ORIGINE_CELLS_Y_I               (80)

#define ORIGINE_CELLS_X_E               (18)
#define ORIGINE_CELLS_Y_E               (80)

#define CLOCK_LEFT_POSITION_B           170
#define CLOCK_TOP_POSITION_B            23

#define CLOCK_LEFT_POSITION_I           338
#define CLOCK_TOP_POSITION_I            23

#define CLOCK_LEFT_POSITION_E           674
#define CLOCK_TOP_POSITION_E            23

#define CONTOR_LEFT_POSITION            25
#define CONTOR_TOP_POSITION             23

#define BITMAP_24_BITS                  3
#define BITMAP_32_BITS                  4

#define NO_MAXIM_BOMBS_B                0x000100
#define NO_MAXIM_BOMBS_I                0x000400
#define NO_MAXIM_BOMBS_E                0x000909


#define NO_COLOANE_GAME_BEGINER         9
#define NO_RANDURI_GAME_EGINER          9

#define NO_COLOANE_GAME_INTERMEDIATE    16
#define NO_RANDURI_GAME_INTERMEDIATE    16

#define NO_COLOANE_GAME_EXPERT          30
#define NO_RANDURI_GAME_EXPERT          16

#define L_CAREU                         24

#define INDEX_EMPTY                     0
#define INDEX_1                         1
#define INDEX_2                         2
#define INDEX_3                         3
#define INDEX_4                         4
#define INDEX_5                         5
#define INDEX_6                         6
#define INDEX_NORMAL                    7
#define INDEX_FLAG                      8
#define INDEX_BOMB                      9
#define INDEX_NO_BOMB                   10
#define INDEX_EXPLODED_BOMB             11

#define INDEX_MUT_NORMAL                12
#define INDEX_MUT_OCHELARI              13
#define INDEX_MUT_SUPARAT               14
#define INDEX_MUT_APASAT                15

#define INDEX_BCK_BEGINER               16
#define INDEX_BCK_INTERMEDIATE          17
#define INDEX_BCK_EXPERT                18

#define NO_BOMBS_BEGINER                10
#define NO_BOMBS_INTERMEDIATE           40
#define NO_BOMBS_EXPERT                 99

#define BEGINER                         1
#define INTERMEDIATE                    2
#define EXPERT                          3

#define SUBFOLDER_LOGS                  "\\Logs\\"
#define FILE_LOGURI                     "logs.log"


using namespace std;


/** clasa de baza abstracta
// toate metodele sunt virtuale pure
//===========================================
**/
class CImage{
    public:
        std::string                     strLogo;
        std::string                     sTag;
        int                             intTag;

    protected:
        unsigned int                    width_;
        unsigned int                    height_;
        unsigned int                    row_increment_;
        unsigned int                    bytes_per_pixel_;
        std::string                     file_name_;
        std::vector<unsigned char>      data_;

    public:

        CImage(){
            strLogo = "Acesta este constructorul clasei CImage! ";
        }

        virtual inline void display_header_of_file(void) = 0;
        virtual inline void display_header_of_bitmap(void) = 0;
        virtual inline void draw_bitmap_on_hdc(const POINT Origine, const HDC hDC) = 0;
        virtual inline void draw_bitmap_on_control(const HWND hwnd) = 0;
        virtual inline void draw_circle_into_bitmap(const POINT Origin, const unsigned int R, const COLORREF color, unsigned int thickness = 1) = 0;
        virtual inline void convert_to_grayscale_by_average(void) = 0;
        virtual inline void convert_to_grayscale_by_weight(void) = 0;
        //virtual inline void expand_imageX2(const CBitmap* image) = 0;
        //virtual inline void flipHX2_image(const CBitmap* image) = 0;
        //virtual inline void flipVX2_image(const CBitmap* image) = 0;
        //virtual inline void extract_bitmaps_into_collection_of_bitmaps(const POINT Origine, const int l_bmp, const int n_H, const int n_V, vector<CBitmap>& tmp_buf1) = 0;
        //virtual inline void extract_bitmaps_into_collection_of_bitmaps_ext(const POINT Origine, const int l_bmp, const int n_H, const int n_V, vector<CBitmap>& tmp_buf1) = 0;
        //virtual inline void extract_bitmaps_into_one_bitmap(const POINT Origine, const int l_bmp, const int n_H, const int n_V, CBitmap& tmp_bitmap) = 0;
        //virtual inline void compare_bitmaps(const int idx1, const int idx2, std::vector<POINT>& res_comp) = 0;
        virtual inline void detele_bitmap_from_collection(const int idx1) = 0;
        virtual inline void swap_bitmaps(const int idx1, const int idx2) = 0;
        virtual inline void save_bitmap(const std::string name_of_file) = 0;

        virtual inline unsigned int get_width(void) = 0;
        virtual inline unsigned int get_height(void) = 0;
        virtual inline unsigned int get_size_of_data(void) = 0;
        virtual inline unsigned int get_this(void) = 0;
        virtual inline void get_file_name(std::ostringstream& strTag) = 0;
        virtual inline unsigned int get_size_bfh(void) = 0;
        virtual inline unsigned int get_size_bih(void) = 0;
        virtual inline unsigned int get_no_of_bytes_per_pixel(void) = 0;

};

/** clasa derivata din clasa CImage
//==================================
 **/
 class CBitmap: public CImage{

    public:

        /**constructor default
        //===================================================
        **/
        CBitmap(void){
                file_name_ = "";
                width_ = 0;
                height_ = 0;
                row_increment_ = 0;
                bytes_per_pixel_ = 0;
        }

        /**constructor de incarcare
        //===================================================
        **/
        CBitmap(const std::string file_name){

            FILE*                           pFile;
            std::vector<unsigned char>      buffer_;

            file_name_= file_name;
            pFile = fopen(&file_name_[0], "rb");
            if(pFile == NULL){cout << "Eroarea: A aparut o eroare la incarcarea fisierului !" << file_name_ << endl; return;}

            /**citeste datele pentru header File
            //============================================================
            **/
            fseek (pFile, 0, SEEK_SET);
            fread (&bfh.TypeOfFile, 1, 2, pFile);

            fseek ( pFile, 2, SEEK_SET );
            fread (&bfh.LengthOfFile, 1, 4, pFile);
            fseek ( pFile, 10, SEEK_SET );
            fread (&bfh.OffsetBMP, 1, 4, pFile);

            /**citeste datele pentru header Info
            //============================================================
            **/
            fseek ( pFile , 14 , SEEK_SET );
            fread (&bih.SizeOfHeader , 1 , 4 , pFile);

            fseek ( pFile, 18, SEEK_SET );
            fread(&bih.Width, 1, 4 , pFile);
            width_ = bih.Width;

            fseek ( pFile, 22, SEEK_SET );
            fread (&bih.Height,1, 4 , pFile);
            height_ = bih.Height;

            fseek ( pFile , 26 , SEEK_SET );
            fread (&bih.NoOfPlanes, 1, 2 , pFile);

            fseek ( pFile, 28, SEEK_SET );
            fread (&bih.NoOfBitsPerPixel, 1, 2 , pFile);

            bytes_per_pixel_ = bih.NoOfBitsPerPixel/8;
            row_increment_ = width_ * bytes_per_pixel_;

            buffer_.resize(row_increment_ * height_);
            fseek ( pFile , bfh.OffsetBMP , SEEK_SET );
            intTag = fread(&buffer_[0], 1, (bfh.LengthOfFile-bfh.OffsetBMP), pFile);
            if (intTag == 0) {cout << "Imaginea nu a fost copiata in memorie!" << endl; return;}

            //display_header_of_file();

            /**inverseaza imaginea fata de axa OX si transform-o in 32 de biti;
            //===================================================================
            **/
            for(unsigned int y=1; y<=height_; y++){
                for(unsigned int x=0; x<width_; x++){
                    data_.push_back(buffer_[(height_-y)*row_increment_+bytes_per_pixel_*x]);
                    data_.push_back(buffer_[(height_-y)*row_increment_+bytes_per_pixel_*x+1]);
                    data_.push_back(buffer_[(height_-y)*row_increment_+bytes_per_pixel_*x+2]);
                    if (bytes_per_pixel_==3) {data_.push_back(0x00);}else{
                       data_.push_back(buffer_[(height_-y)*row_increment_+bytes_per_pixel_*x+3]);
                    }
                }
            }
            bytes_per_pixel_=4;
            row_increment_=width_ * bytes_per_pixel_;
            fclose(pFile);

            //display_header_of_bitmap();
        }

        /**constructor
        //================================================================
        **/
        CBitmap(const unsigned int width, const unsigned int height){
            width_ = width;
            height_ = height;
            bytes_per_pixel_ = 3;
            row_increment_ = width_ * bytes_per_pixel_;

            bih.Width = width_;
            bih.Height = height_;
            bih.NoOfBitsPerPixel = bytes_per_pixel_*8;
            for(unsigned int x=0; x<width_*height_*bytes_per_pixel_; x++){
                data_.push_back(0x00);
            }
        }

        /** creaza un Bitmap de a anumita culoare
        //======================================================
        **/
        CBitmap(const unsigned int width, const unsigned int height, const COLORREF color){
            width_ = width;
            height_ = height;
            bytes_per_pixel_ = 4;
            row_increment_ = width_ * bytes_per_pixel_;
            //data_.reserve(width_*height_*bytes_per_pixel_);
            for(unsigned int x=0; x<width_*height_; x++){
                data_.push_back((static_cast<unsigned char>(color>>24)));
                data_.push_back((static_cast<unsigned char>(color>>16)));
                data_.push_back((static_cast<unsigned char>(color>>8)));
                data_.push_back((static_cast<unsigned char>(color)));
            }
        }


        /**supraincarcarea operatorului =
        //================================================================
        **/
        CBitmap& operator = (const CBitmap& image){
            if (this != &image){
                width_ = image.width_;
                height_ = image.height_ ;
                bytes_per_pixel_ = image.bytes_per_pixel_;
                bih.Width = width_;
                bih.Height = height_;
                bih.NoOfBitsPerPixel = bytes_per_pixel_*8;
                row_increment_ = image.row_increment_;
                data_ = image.data_;
            }
            //return this;
        }


        /** definirea desctructorului
        //================================================================
        **/
        /*
        ~CBitmap(){
            if (this!=nullptr){delete this;}
        }
        */


        /**functiile clasei
        //deseneaza bitmap-ul din memorie pe un context de dispozitiv
        //================================================================
        **/
        virtual inline void draw_bitmap_on_hdc(const POINT Origine, const HDC hDC){
            int         R, G, B, A;

            for(unsigned int y=0; y<height_; y++){
                for(unsigned int x=0; x<width_; x++){
                    B = data_[y*row_increment_+bytes_per_pixel_*x];
                    G = data_[y*row_increment_+bytes_per_pixel_*x+1];
                    R = data_[y*row_increment_+bytes_per_pixel_*x+2];
                    A = data_[y*row_increment_+bytes_per_pixel_*x+3];
                    //SetPixelV(hDC, Origine.x + x , Origine.y + height_ - y, RGB(R, G, B));
                    SetPixelV(hDC, Origine.x + x , Origine.y + y, RGB(R, G, B));
                }
            }
            //display_header();
        }

        /**
        //deseneaza bitmap-ul din memorie pe un context de dispozitiv
        //================================================================
        **/

        virtual inline void draw_bitmap_on_control(const HWND hwnd){

            BITMAP                  bmp;
            HBITMAP                 hbmp;

            bmp.bmType = 19778;
            bmp.bmWidth = width_;
            bmp.bmHeight = height_;
            bmp.bmWidthBytes = width_*4;
            bmp.bmPlanes = 1;
            bmp.bmBitsPixel = 32;
            bmp.bmBits = data_.data();
            hbmp = CreateBitmapIndirect(&bmp);

            if (hbmp == NULL){ std::cout << "Eroare: Sistemul de operare nu a alocat nici un handler pentru bitmap !" << endl;}
            //else{cout << "HBITMAP: " << hbmp << "\n";}
            DWORD err = GetLastError();
            if (err!=0){cout << "err: " << err << "\n";}
            //InvalidateRect(hwnd,NULL,true);
            //::SelectObject(hdcStatic,(HBITMAP)bmp2);
            SendMessage(hwnd, STM_SETIMAGE , (WPARAM)IMAGE_BITMAP, (LPARAM)hbmp);
        }


        virtual inline HBITMAP generate_HBITMAP(void){

            BITMAP                  bmp;
            HBITMAP                 hbmp;

            bmp.bmType = 19778;
            bmp.bmWidth = width_;
            bmp.bmHeight = height_;
            bmp.bmWidthBytes = width_*4;
            bmp.bmPlanes = 1;
            bmp.bmBitsPixel = 32;
            bmp.bmBits = data_.data();
            hbmp = CreateBitmapIndirect(&bmp);

            if (hbmp == NULL){std::cout << "Eroare: Sistemul de operare nu a alocat nici un handler pentru bitmap !" << endl;}

            return hbmp;
        }


        /**
        //deseneaza un cerc in bitmap
        //==============================================================
        **/
        virtual inline void draw_circle_into_bitmap(const POINT Origin, const unsigned int R, const COLORREF color, unsigned int thickness = 1){

            int                     x, y, i, j, shift, raza;
            double                  k;
            POINT                   point;
            std::vector<POINT>      buffer;


            if(((Origin.x <= R) || (width_-Origin.x <= R)) || ((Origin.y <= R) || (height_-Origin.y <= R))){
                cout << "Eroare: Cercul nu se poate desena !\Iese in afara imaginii !" << endl;
                return;
            }

            thickness = 20;
            raza = R-thickness/2;

            for (j=0;j<thickness;j++){
                for (x=0; x<raza ;x++){
                    k = static_cast<double>(x)/static_cast<double>(raza);
                    k = k*k;
                    y = raza*sqrt(1-k);

                    point.x = Origin.x+x;
                    point.y = Origin.y+y;
                    buffer.push_back(point);
                    point.x = Origin.x+x;
                    point.y = Origin.y-y;
                    buffer.push_back(point);
                    point.x = Origin.x-x;
                    point.y = Origin.y+y;
                    buffer.push_back(point);
                    point.x = Origin.x-x;
                    point.y = Origin.y-y;
                    buffer.push_back(point);
                }

                for (y=0; y<raza ;y++){
                    k = static_cast<double>(y)/static_cast<double>(raza);
                    k = k*k;
                    x=raza*sqrt(1-k);

                    point.x = Origin.x+x;
                    point.y = Origin.y+y;
                    buffer.push_back(point);
                    point.x = Origin.x+x;
                    point.y = Origin.y-y;
                    buffer.push_back(point);
                    point.x = Origin.x-x;
                    point.y = Origin.y+y;
                    buffer.push_back(point);
                    point.x = Origin.x-x;
                    point.y = Origin.y-y;
                    buffer.push_back(point);
                }
                raza++;
            }
            for(int j=0; j<buffer.size(); j++){
                point=buffer[j];
                i = point.y*row_increment_+point.x*bytes_per_pixel_;
                if (bytes_per_pixel_==4){
                    shift=1;
                    //Dump = data_[y*row_increment_+bytes_per_pixel_*x];
                    data_[i] = static_cast<unsigned char>(color>>24);
                }else{shift=0;}
                data_[i+shift] = static_cast<unsigned char>(color>>16);
                data_[i+shift+1] = static_cast<unsigned char>(color>>8);
                data_[i+shift+2] = static_cast<unsigned char>(color);
            }
        }


        virtual inline void convert_to_grayscale_by_average(void){

            unsigned char           cTag[4];
            int                     shift;
            int unsigned            media;

            cTag[0]=0;
            for(unsigned int i=0; i<width_*height_; i++){
                if (bytes_per_pixel_==4){shift=1;}else{shift=0;}

                for(unsigned int j=shift; j<bytes_per_pixel_; j++){
                    cTag[j] = data_[j+i*bytes_per_pixel_];
                }
                media=static_cast<unsigned char>(static_cast<int>(cTag[0]+cTag[1]+cTag[2]+cTag[3])/bytes_per_pixel_);
                for(int j=shift; j<bytes_per_pixel_; j++){
                    data_[j+i*bytes_per_pixel_]=media;
                }
            }
        }


        virtual inline void convert_to_grayscale_by_weight(void){

            double r_scaler = 0.299;
            double g_scaler = 0.587;
            double b_scaler = 0.114;


            for (unsigned char* itr = data_.data(); itr < data_.data() + data_.size(); )
            {
                unsigned char gray_value = static_cast<unsigned char>
                       (
                         (r_scaler * (*(itr + 2))) +
                         (g_scaler * (*(itr + 1))) +
                         (b_scaler * (*(itr + 0)))
                       );

                *(itr++) = gray_value;
                *(itr++) = gray_value;
                *(itr++) = gray_value;
            }
        }


        virtual inline void expand_imageX2(const CBitmap& image){

            unsigned char                       R,G,B,A;
            std::vector<unsigned char>          row_buffer;
            int                                 idx1,idx2;

            width_ = 2*image.width_-1;
            height_ = image.height_;
            bytes_per_pixel_ = image.bytes_per_pixel_;
            row_increment_ = width_*bytes_per_pixel_;

            /** scaleaza pe orizontala
            //============================================================
            **/
            for (unsigned int y=0; y<image.height_;y++){
                for(unsigned int x=0; x<image.width_;x++){
                    idx1=image.row_increment_*y+image.bytes_per_pixel_*x;
                    idx2=image.row_increment_*y+image.bytes_per_pixel_*(x-1);

                    if (x==0){
                        data_.push_back(image.data_[idx1]);
                        data_.push_back(image.data_[idx1+1]);
                        data_.push_back(image.data_[idx1+2]);
                        data_.push_back(image.data_[idx1+3]);
                    }else{

                        B = (image.data_[idx2]+image.data_[idx1])/2;
                        G = (image.data_[idx2+1]+image.data_[idx1+1])/2;
                        R = (image.data_[idx2+2]+image.data_[idx1+2])/2;
                        A = (image.data_[idx2+3]+image.data_[idx1+3])/2;

                        data_.push_back(B);
                        data_.push_back(G);
                        data_.push_back(R);
                        data_.push_back(A);

                        data_.push_back(image.data_[idx1]);
                        data_.push_back(image.data_[idx1+1]);
                        data_.push_back(image.data_[idx1+2]);
                        data_.push_back(image.data_[idx1+3]);

                    }
                }
            }


            /**scaleaza pe verticala
            //===============================================================
            **/
            std::vector<unsigned char>::iterator    it;

            for (unsigned int y=1; y<height_;y++){
                it=data_.begin()+(height_-y)*row_increment_;
                for(unsigned int x=0; x<width_;x++){
                    idx1=(height_-y)*row_increment_+bytes_per_pixel_*x;
                    idx2=(height_-y-1)*row_increment_+bytes_per_pixel_*x;

                    B = (data_[idx2]+data_[idx1])/2;
                    G = (data_[idx2+1]+data_[idx1+1])/2;
                    R = (data_[idx2+2]+data_[idx1+2])/2;
                    A = (data_[idx2+3]+data_[idx1+3])/2;

                    row_buffer.push_back(B);
                    row_buffer.push_back(G);
                    row_buffer.push_back(R);
                    row_buffer.push_back(A);

                }
                data_.insert(it,row_buffer.begin(),row_buffer.end());
                row_buffer.clear();
            }
            height_ = 2*image.height_-1;
        }


        virtual inline void flipHX2_image(const CBitmap& image){

            unsigned char                   R,G,B,A;
            std::vector<unsigned char>      row_buffer;
            int                             idx1,idx2;

            width_ = 2*image.width_-1;
            height_ = image.height_;
            bytes_per_pixel_ = image.bytes_per_pixel_;
            row_increment_ = width_*bytes_per_pixel_;

            //data_.reserve(100000);
            /** scaleaza pe orizontala
            //============================================================
            **/
            for (unsigned int y=0; y<image.height_;y++){
                for(unsigned int x=0; x<image.width_;x++){
                    idx1=image.row_increment_*y+image.bytes_per_pixel_*x;
                    idx2=image.row_increment_*y+image.bytes_per_pixel_*(x-1);

                    if (x==0){
                        data_.push_back(image.data_[idx1]);
                        data_.push_back(image.data_[idx1+1]);
                        data_.push_back(image.data_[idx1+2]);
                        data_.push_back(image.data_[idx1+3]);
                    }else{
                        B = (image.data_[idx2]+image.data_[idx1])/2;
                        G = (image.data_[idx2+1]+image.data_[idx1+1])/2;
                        R = (image.data_[idx2+2]+image.data_[idx1+2])/2;
                        A = (image.data_[idx2+3]+image.data_[idx1+3])/2;

                        data_.push_back(B);
                        data_.push_back(G);
                        data_.push_back(R);
                        data_.push_back(A);

                        data_.push_back(image.data_[idx1]);
                        data_.push_back(image.data_[idx1+1]);
                        data_.push_back(image.data_[idx1+2]);
                        data_.push_back(image.data_[idx1+3]);

                    }
                }
            }
        }


        virtual inline void flipVX2_image(const CBitmap& image){

            unsigned char                           R,G,B,A;
            int                                     idx1,idx2;
            std::vector<unsigned char>              row_buffer;
            std::vector<unsigned char>::iterator    it;


            width_ = image.width_;
            height_ = image.height_;
            bytes_per_pixel_ = image.bytes_per_pixel_;
            row_increment_ = width_*bytes_per_pixel_;
            data_=image.data_;

            /**scaleaza pe verticala
            //===============================================================
            **/

            for (unsigned int y=1; y<height_;y++){
                it=data_.begin()+(height_-y)*row_increment_;
                for(unsigned int x=0; x<width_;x++){
                    idx1=(height_-y)*row_increment_+bytes_per_pixel_*x;
                    idx2=(height_-y-1)*row_increment_+bytes_per_pixel_*x;

                    B = (data_[idx2]+data_[idx1])/2;
                    G = (data_[idx2+1]+data_[idx1+1])/2;
                    R = (data_[idx2+2]+data_[idx1+2])/2;
                    A = (data_[idx2+3]+data_[idx1+3])/2;
                    //printf("B:%x  G:%x   R:%x\n",B,G,R);

                    row_buffer.push_back(B);
                    row_buffer.push_back(G);
                    row_buffer.push_back(R);
                    row_buffer.push_back(A);

                }
                data_.insert(it,row_buffer.begin(),row_buffer.end());
                row_buffer.clear();
            }
            height_ = 2*image.height_-1;
        }


        virtual inline void extract_bitmaps_into_collection_of_bitmaps(const POINT Origine, const int l_bmp, const int n_H, const int n_V, vector<CBitmap>& tmp_buf1){

            unsigned int            idxl;
            unsigned char           R,G,B,A;
            CBitmap                 tmp_bmp;
            vector<CBitmap>         tmp_buf;

            tmp_bmp.width_ = l_bmp;
            tmp_bmp.height_ = l_bmp;
            tmp_bmp.bytes_per_pixel_ = 4;
            tmp_bmp.row_increment_ = tmp_bmp.bytes_per_pixel_*tmp_bmp.height_;

            display_header_of_bitmap();
            /*
            cout << "width sursa: " << width_ << endl;
            cout << "height sursa: " << height_ << endl;
            cout << "Origine.x+l_bmp*n_H: " << (Origine.x+l_bmp*n_H) << endl;
            cout << "Origine.y+l_bmp*n_V: " << (Origine.y+l_bmp*n_V) << endl;
            cout << "data_size(): " << (data_.size()) << endl;
            //cout << "data_size(): " << (bmp_sursa.data_.size()) << endl;
            */
            if ((width_<Origine.x+l_bmp*n_H) || (height_<Origine.y+l_bmp*n_V)){
                cout << "Eroare: Nu pot fi extrase bitmap-urile\nModificati parametrii Origine, l_bmp, n_H sau n_V ai functiei !" << endl;
            }
            for(int y=0; y<height_;y++){
                for(int x=0; x<width_;x++){
                    if ((y>=Origine.y) && (x>=Origine.x)){
                        if((((y-Origine.y)%l_bmp == 0) && ((x-Origine.x)%l_bmp == 0)) && ((x-Origine.x<n_H*l_bmp) && (y-Origine.y<n_V*l_bmp))){
                            for(int y1=0; y1<l_bmp;y1++){
                                for(int x1=0; x1<l_bmp;x1++){
                                    idxl=(y+y1)*row_increment_+(x+x1)*bytes_per_pixel_;
                                    B=data_[idxl];
                                    G=data_[idxl+1];
                                    R=data_[idxl+2];
                                    A=0x00;
                                    tmp_bmp.data_.push_back(B);
                                    tmp_bmp.data_.push_back(G);
                                    tmp_bmp.data_.push_back(R);
                                    tmp_bmp.data_.push_back(A);
                                }
                            }
                            tmp_buf1.push_back(tmp_bmp);
                            tmp_bmp.data_.clear();
                        }
                    }
                }
            }
        }


        virtual inline void extract_bitmaps_into_collection_of_bitmaps_ext(const POINT Origine, const int l_bmp, const int n_H, const int n_V, vector<CBitmap>& tmp_buf1){

            std::vector<CBitmap>            tmp_buf;
            CBitmap                         tmp_bmp;
            unsigned int                    idxl;
            unsigned char                   R,G,B,A;
            bool                            bTag;

            tmp_bmp.width_ = l_bmp;
            tmp_bmp.height_ = l_bmp;
            tmp_bmp.bytes_per_pixel_ = 4;
            tmp_bmp.row_increment_ = tmp_bmp.bytes_per_pixel_*tmp_bmp.height_;

            //display_header_of_bitmap();

            if ((width_<Origine.x+l_bmp*n_H) || (height_<Origine.y+l_bmp*n_V)){
                cout << "Eroare: Nu pot fi extrase bitmap-urile\nModificati parametrii Origine, l_bmp, n_H sau n_V ai functiei !" << endl;
            }
            for(int y=0; y<height_;y++){
                for(int x=0; x<width_;x++){
                    if ((y>=Origine.y) && (x>=Origine.x)){
                        if((((y-Origine.y)%l_bmp == 0) && ((x-Origine.x)%l_bmp == 0)) && ((x-Origine.x<n_H*l_bmp) && (y-Origine.y<n_V*l_bmp))){
                            for(int y1=0; y1<l_bmp;y1++){
                                for(int x1=0; x1<l_bmp;x1++){
                                    idxl=(y+y1)*row_increment_+(x+x1)*bytes_per_pixel_;
                                    B=data_[idxl];
                                    G=data_[idxl+1];
                                    R=data_[idxl+2];
                                    A=0x00;
                                    tmp_bmp.data_.push_back(B);
                                    tmp_bmp.data_.push_back(G);
                                    tmp_bmp.data_.push_back(R);
                                    tmp_bmp.data_.push_back(A);
                                }
                            }
                            tmp_buf.push_back(tmp_bmp);
                            tmp_bmp.data_.clear();
                        }
                    }
                }
            }
            tmp_bmp=tmp_buf[0];
            tmp_buf1.push_back(tmp_bmp);
            if (tmp_buf.size()==1){return;}

            for (int i=0; i<tmp_buf.size(); i++){
                bTag=true;
                for (int j=0; j<tmp_buf1.size(); j++){
                    for (int k=0; k<tmp_buf1[j].data_.size();k++){
                        if(tmp_buf1[j].data_[k]!=tmp_buf[i].data_[k]){
                            bTag=false;
                            goto LABEL_1;
                        }
                    }
                    goto LABEL_2;
LABEL_1:;
                }
                if (!(bTag)){
                    tmp_bmp=tmp_buf[i];
                    tmp_buf1.push_back(tmp_bmp);
                }
LABEL_2:;
            }
        }



        virtual inline void extract_bitmaps_into_one_bitmap(const POINT Origine, const int l_bmp, const int n_H, const int n_V, CBitmap& tmp_bitmap){

            std::vector<CBitmap>            tmp_buf, tmp_buf1;
            CBitmap                         tmp_bmp;
            unsigned int                    idxl, k;
            unsigned char                   R,G,B,A;
            bool                            bTag;

            tmp_bmp.width_ = l_bmp;
            tmp_bmp.height_ = l_bmp;
            tmp_bmp.bytes_per_pixel_ = 4;
            tmp_bmp.row_increment_ = tmp_bmp.bytes_per_pixel_*tmp_bmp.width_;

            //display_header_of_bitmap();

            if ((width_<Origine.x+l_bmp*n_H) || (height_<Origine.y+l_bmp*n_V)){
                cout << "Eroare: Nu pot fi extrase bitmap-urile\n" << "        Modificati parametrii Origine, l_bmp, n_H sau n_V ai functiei\n" << endl;
            }
            for(int y=0; y<height_;y++){
                for(int x=0; x<width_;x++){
                    if ((y>=Origine.y) && (x>=Origine.x)){
                        if((((y-Origine.y)%l_bmp == 0) && ((x-Origine.x)%l_bmp == 0)) && ((x-Origine.x<n_H*l_bmp) && (y-Origine.y<n_V*l_bmp))){
                            for(int y1=0; y1<l_bmp;y1++){
                                for(int x1=0; x1<l_bmp;x1++){
                                    idxl=(y+y1)*row_increment_+(x+x1)*bytes_per_pixel_;
                                    B=data_[idxl];
                                    G=data_[idxl+1];
                                    R=data_[idxl+2];
                                    A=0x00;
                                    tmp_bmp.data_.push_back(B);
                                    tmp_bmp.data_.push_back(G);
                                    tmp_bmp.data_.push_back(R);
                                    tmp_bmp.data_.push_back(A);
                                }
                            }
                            tmp_buf.push_back(tmp_bmp);
                            tmp_bmp.data_.clear();
                        }
                    }
                }
            }
            tmp_bmp=tmp_buf[0];
            tmp_buf1.push_back(tmp_bmp);
            if (tmp_buf.size()==1){goto LABEL_3;}

            for (int i=0; i<tmp_buf.size(); i++){
                bTag=true;
                for (int j=0; j<tmp_buf1.size(); j++){
                    for (int k=0; k<tmp_buf1[j].data_.size();k++){
                        if(tmp_buf1[j].data_[k]!=tmp_buf[i].data_[k]){
                            bTag=false;
                            goto LABEL_1;
                        }
                    }
                    goto LABEL_2;
LABEL_1:;
                }
                if (!(bTag)){
                    tmp_bmp=tmp_buf[i];
                    tmp_buf1.push_back(tmp_bmp);
                }
LABEL_2:;
            }
LABEL_3:
            cout << "tmp_buf1.size(): " << tmp_buf1.size() << endl;
            k=tmp_buf1.size();
            tmp_bitmap.width_ = k*tmp_bmp.width_;
            tmp_bitmap.height_ = tmp_bmp.height_;
            tmp_bitmap.row_increment_ = k*tmp_bmp.row_increment_;
            tmp_bitmap.bytes_per_pixel_ = tmp_bmp.bytes_per_pixel_;

            for(int y=0; y<l_bmp; y++){
                for(int i=0; i<tmp_buf1.size(); i++){
                    for(int x=0; x<l_bmp; x++){
                        idxl=y*tmp_bmp.row_increment_+x*tmp_bmp.bytes_per_pixel_;
                        B=tmp_buf1[i].data_[idxl];
                        G=tmp_buf1[i].data_[idxl+1];
                        R=tmp_buf1[i].data_[idxl+2];
                        A=tmp_buf1[i].data_[idxl+3];
                        tmp_bitmap.data_.push_back(B);
                        tmp_bitmap.data_.push_back(G);
                        tmp_bitmap.data_.push_back(R);
                        tmp_bitmap.data_.push_back(A);
                    }
                }
            }
            //cout << tmp_bitmap.data_.size() << endl;
        }




 virtual inline void extract_bitmaps_into_one_bitmapE(const POINT Origine, const int L_bmp, const int H_bmp, const int n_L, const int n_H, CBitmap& tmp_bitmap){

            std::vector<CBitmap>            tmp_buf, tmp_buf1;
            CBitmap                         tmp_bmp;
            unsigned int                    idxl, k;
            unsigned char                   R,G,B,A;
            bool                            bTag;

            tmp_bmp.width_ = L_bmp;
            tmp_bmp.height_ = H_bmp;
            tmp_bmp.bytes_per_pixel_ = 4;
            tmp_bmp.row_increment_ = tmp_bmp.bytes_per_pixel_*tmp_bmp.width_;

            //display_header_of_bitmap();

            if ((width_<Origine.x+L_bmp*n_L) || (height_<Origine.y+H_bmp*n_H)){
                cout << "Eroare: Nu pot fi extrase bitmap-urile\n" << "        Modificati parametrii Origine, l_bmp, n_H sau n_V ai functiei\n" << endl;
            }
            for(int y=0; y<height_;y++){
                for(int x=0; x<width_;x++){
                    if ((y>=Origine.y) && (x>=Origine.x)){
                        if((((y-Origine.y)%H_bmp == 0) && ((x-Origine.x)%L_bmp == 0)) && ((x-Origine.x<n_L*L_bmp) && (y-Origine.y<n_H*H_bmp))){
                            for(int y1=0; y1<H_bmp;y1++){
                                for(int x1=0; x1<L_bmp;x1++){
                                    idxl=(y+y1)*row_increment_+(x+x1)*bytes_per_pixel_;
                                    B=data_[idxl];
                                    G=data_[idxl+1];
                                    R=data_[idxl+2];
                                    A=0x00;
                                    tmp_bmp.data_.push_back(B);
                                    tmp_bmp.data_.push_back(G);
                                    tmp_bmp.data_.push_back(R);
                                    tmp_bmp.data_.push_back(A);
                                }
                            }
                            tmp_buf.push_back(tmp_bmp);
                            tmp_bmp.data_.clear();
                        }
                    }
                }
            }

            //tmp_bitmap = tmp_buf[0];
            //return;
            tmp_bmp=tmp_buf[0];
            tmp_buf1.push_back(tmp_bmp);
            if (tmp_buf.size()==1){goto LABEL_3;}

            for (int i=0; i<tmp_buf.size(); i++){
                bTag=true;
                for (int j=0; j<tmp_buf1.size(); j++){
                    for (int k=0; k<tmp_buf1[j].data_.size();k++){
                        if(tmp_buf1[j].data_[k]!=tmp_buf[i].data_[k]){
                            bTag=false;
                            goto LABEL_1;
                        }
                    }
                    goto LABEL_2;
LABEL_1:;
                }
                if (!(bTag)){
                    tmp_bmp=tmp_buf[i];
                    tmp_buf1.push_back(tmp_bmp);
                }
LABEL_2:;
            }
LABEL_3:
            cout << "tmp_buf1.size(): " << tmp_buf1.size() << endl;
            k=tmp_buf1.size();
            tmp_bitmap.width_ = k*tmp_bmp.width_;
            tmp_bitmap.height_ = tmp_bmp.height_;
            tmp_bitmap.row_increment_ = k*tmp_bmp.row_increment_;
            tmp_bitmap.bytes_per_pixel_ = tmp_bmp.bytes_per_pixel_;

            for(int y=0; y<H_bmp; y++){
                for(int i=0; i<tmp_buf1.size(); i++){
                    for(int x=0; x<L_bmp; x++){
                        idxl=y*tmp_bmp.row_increment_+x*tmp_bmp.bytes_per_pixel_;
                        B=tmp_buf1[i].data_[idxl];
                        G=tmp_buf1[i].data_[idxl+1];
                        R=tmp_buf1[i].data_[idxl+2];
                        A=tmp_buf1[i].data_[idxl+3];
                        tmp_bitmap.data_.push_back(B);
                        tmp_bitmap.data_.push_back(G);
                        tmp_bitmap.data_.push_back(R);
                        tmp_bitmap.data_.push_back(A);
                    }
                }
            }
            //cout << tmp_bitmap.data_.size() << endl;
        }





   /**

        virtual inline void extract_bitmaps_into_collection_of_bitmapsE(const POINT Origine, const int L_bmp, const int H_bmp, const int n_L, const int n_H, vector<CBitmap>& tmp_buf1){

            std::vector<CBitmap>            tmp_buf;
            CBitmap                         tmp_bmp;
            unsigned int                    idxl;
            unsigned char                   R,G,B,A;
            bool                            bTag;

            tmp_bmp.width_ = L_bmp;
            tmp_bmp.height_ = H_bmp;
            tmp_bmp.bytes_per_pixel_ = 4;
            tmp_bmp.row_increment_ = tmp_bmp.bytes_per_pixel_*tmp_bmp.height_;

            display_header_of_bitmap();

            if ((width_<Origine.x+L_bmp*n_L) || (height_<Origine.y+H_bmp*n_H)){
                cout << "Eroare: Nu pot fi extrase bitmap-urile\nModificati parametrii Origine, l_bmp, n_H sau n_V ai functiei !" << endl;
            }
            for(int y=0; y<height_;y++){
                for(int x=0; x<width_;x++){
                    if ((y>=Origine.y) && (x>=Origine.x)){
                        if((((y-Origine.y)%H_bmp == 0) && ((x-Origine.x)%L_bmp == 0)) && ((x-Origine.x<n_L*L_bmp) && (y-Origine.y<n_H*H_bmp))){
                            //cout << "x: " << x << "y: " << y << endl;
                            for(int y1=0; y1<H_bmp; y1++){
                                for(int x1=0; x1<L_bmp; x1++){
                                    idxl=(y+y1)*row_increment_+(x+x1)*bytes_per_pixel_;
                                    B=data_[idxl];
                                    G=data_[idxl+1];
                                    R=data_[idxl+2];
                                    A=0x00;
                                    tmp_bmp.data_.push_back(B);
                                    tmp_bmp.data_.push_back(G);
                                    tmp_bmp.data_.push_back(R);
                                    tmp_bmp.data_.push_back(A);
                                }
                            }
                            tmp_buf.push_back(tmp_bmp);
                            tmp_bmp.data_.clear();
                        }
                    }
                }
            }
            tmp_bmp=tmp_buf[0];
            tmp_buf1.push_back(tmp_bmp);
            if (tmp_buf.size()==1){return;}

            for (int i=0; i<tmp_buf.size(); i++){
                bTag=true;
                for (int j=0; j<tmp_buf1.size(); j++){
                    for (int k=0; k<tmp_buf1[j].data_.size();k++){
                        if(tmp_buf1[j].data_[k]!=tmp_buf[i].data_[k]){
                            bTag=false;
                            goto LABEL_1;
                        }
                    }
                    goto LABEL_2;
LABEL_1:;
                }
                if (!(bTag)){
                    tmp_bmp=tmp_buf[i];
                    tmp_buf1.push_back(tmp_bmp);
                }
LABEL_2:;
            }
        }

**/


        virtual inline void extract_bitmaps_into_collection_of_bitmapsE(const POINT Origine, const int L_bmp, const int H_bmp, const int n_L, const int n_H, vector<CBitmap>& tmp_buf1){

            std::vector<CBitmap>            tmp_buf;
            CBitmap                         tmp_bmp;
            unsigned int                    idxl;
            unsigned char                   R,G,B,A;
            bool                            bTag;

            tmp_bmp.width_ = L_bmp;
            tmp_bmp.height_ = H_bmp;
            tmp_bmp.bytes_per_pixel_ = 4;
            tmp_bmp.row_increment_ = tmp_bmp.bytes_per_pixel_*tmp_bmp.width_;

            //display_header_of_bitmap();

            if ((width_<Origine.x+L_bmp*n_L) || (height_<Origine.y+H_bmp*n_H)){
                cout << "Eroare: Nu pot fi extrase bitmap-urile\n" << "        Modificati parametrii Origine, l_bmp, n_H sau n_V ai functiei\n" << endl;
            }
            for(int y=0; y<height_;y++){
                for(int x=0; x<width_;x++){
                    if ((y>=Origine.y) && (x>=Origine.x)){
                        if((((y-Origine.y)%H_bmp == 0) && ((x-Origine.x)%L_bmp == 0)) && ((x-Origine.x<n_L*L_bmp) && (y-Origine.y<n_H*H_bmp))){
                            for(int y1=0; y1<H_bmp;y1++){
                                for(int x1=0; x1<L_bmp;x1++){
                                    idxl=(y+y1)*row_increment_+(x+x1)*bytes_per_pixel_;
                                    B=data_[idxl];
                                    G=data_[idxl+1];
                                    R=data_[idxl+2];
                                    A=0x00;
                                    tmp_bmp.data_.push_back(B);
                                    tmp_bmp.data_.push_back(G);
                                    tmp_bmp.data_.push_back(R);
                                    tmp_bmp.data_.push_back(A);
                                }
                            }
                            tmp_buf.push_back(tmp_bmp);
                            tmp_bmp.data_.clear();
                        }
                    }
                }
            }

            //tmp_bitmap = tmp_buf[0];
            //return;
            tmp_bmp=tmp_buf[0];
            tmp_buf1.push_back(tmp_bmp);
            if (tmp_buf.size()==1){return;}

            for (int i=0; i<tmp_buf.size(); i++){
                bTag=true;
                for (int j=0; j<tmp_buf1.size(); j++){
                    for (int k=0; k<tmp_buf1[j].data_.size();k++){
                        if(tmp_buf1[j].data_[k]!=tmp_buf[i].data_[k]){
                            bTag=false;
                            goto LABEL_1;
                        }
                    }
                    goto LABEL_2;
LABEL_1:;
                }
                if (!(bTag)){
                    tmp_bmp=tmp_buf[i];
                    tmp_buf1.push_back(tmp_bmp);
                }
LABEL_2:;
            }
        }


        virtual inline void compare_bitmaps(const int idx1, const int idx2, std::vector<POINT>& res_comp){

            std::vector<unsigned char>          tmp_bmp1, tmp_bmp2;
            int                                 idx;


            if ((idx1==0) || (idx2==0)){cout << "Eroare: Nu se accepta index 0; Indexul incepe de la 1 !" << endl;}
            if ((idx1>width_/height_) || (idx2>width_/height_)) {cout << "Eroare: Unul dintre numere sau ambele sunt in afara domeniului acceptat de bitmap !" << endl;}

            for (int y=0; y<height_; y++){
                for(int x=0; x<width_; x++){
                    idx = y*row_increment_+x*bytes_per_pixel_;
                    if (static_cast<int>(x/height_) == (idx1-1)){
                        tmp_bmp1.push_back(data_[idx]);
                        tmp_bmp1.push_back(data_[idx+1]);
                        tmp_bmp1.push_back(data_[idx+2]);
                        if (bytes_per_pixel_==4){tmp_bmp1.push_back(data_[idx+3]);}
                    }
                    if (static_cast<int>(x/height_) == (idx2-1)){
                        tmp_bmp2.push_back(data_[idx]);
                        tmp_bmp2.push_back(data_[idx+1]);
                        tmp_bmp2.push_back(data_[idx+2]);
                        if (bytes_per_pixel_==4){tmp_bmp2.push_back(data_[idx+3]);}
                    }
                }
            }

            for (int y=0; y<height_; y++){
                for(int x=0; x<height_; x++){

                    idx = (y*height_+x)*bytes_per_pixel_;
                    if (bytes_per_pixel_==4){

                        if ((tmp_bmp1[idx]!=tmp_bmp2[idx]) || (tmp_bmp1[idx+1]!=tmp_bmp2[idx+1])||
                            (tmp_bmp1[idx+2]!=tmp_bmp2[idx+2]) || (tmp_bmp1[idx+3]!=tmp_bmp2[idx+3])){
                            res_comp.push_back({x,y});
                        }
                    }else if (bytes_per_pixel_==3){
                        if ((tmp_bmp1[idx]!=tmp_bmp2[idx]) || (tmp_bmp1[idx+1]!=tmp_bmp2[idx+1])||
                            (tmp_bmp1[idx+2]!=tmp_bmp2[idx+2])){
                            res_comp.push_back({x,y});
                        }
                    }
                }
            }
        }

        virtual inline void detele_bitmap_from_collection(const int idx1){

            std::vector<unsigned char>::iterator        it1, it2;
            int                                         idx;


            if ((width_<height_) || (width_%height_!=0)){cout << "Eroare: Bitmap-ul nu reprezinta o colectie de bitmap-uri !" << endl;return;}
            if (idx1==0){cout << "Eroare: Nu se accepta index 0; Indexul incepe de la 1 !" << endl; return;}
            if (idx1>width_/height_) {cout << "Eroare: Indexul este in afara domeniului acceptat de bitmap !" << endl; return;}

            for (int y=0; y<height_; y++){
                for(int x=0; x<width_; x=x+height_){
                    idx = (y*(width_-height_)+x)*bytes_per_pixel_;
                    it1=data_.begin()+idx;
                    it2=it1+height_*bytes_per_pixel_;
                    if(static_cast<int>(x/height_) == (idx1-1)){
                        data_.erase(it1,it2);
                    }
                }
            }

            width_-=height_;
            row_increment_=width_*bytes_per_pixel_;
        }


        virtual inline void swap_bitmaps(const int idx1, const int idx2){

            unsigned char                   R1,G1,B1,A1, R2,G2,B2,A2;

            if ((width_<height_) || (width_%height_!=0)){cout << "Eroare: Bitmap-ul nu este pregatit pentru swap !" << endl;}
            if ((idx1==0) || (idx2==0)){cout << "Eroare: Nu se accepta index 0; Indexul incepe de la 1 !" << endl;}
            if ((idx1>width_/height_) || (idx2>width_/height_)) {cout << "Eroare: Unul dintre numere sau ambele sunt in afara domeniului acceptat de bitmap !" << endl;}

            //cout << "row_increment_: " << row_increment_ << endl;
            //cout << "width_/height_: " << (width_/height_) << endl;
            //cout << "width_*bytes_per_pixel_: " << (width_*bytes_per_pixel_) << endl;

            for(unsigned int y=0; y<height_; y++){
                for(unsigned int x=0; x<width_; x++){
                    if (static_cast<int>(x/height_) == (idx1-1)){
                        //cout << "static_cast<int>(x/height_): " << static_cast<int>(x/height_) << "    " << "x: " << x << "   y:" << y << endl;
                        B1=data_[y*row_increment_+x*bytes_per_pixel_];
                        G1=data_[y*row_increment_+x*bytes_per_pixel_+1];
                        R1=data_[y*row_increment_+x*bytes_per_pixel_+2];
                        A1=data_[y*row_increment_+x*bytes_per_pixel_+3];

                        B2=data_[y*row_increment_+(x+(idx2-idx1)*height_)*bytes_per_pixel_];
                        G2=data_[y*row_increment_+(x+(idx2-idx1)*height_)*bytes_per_pixel_+1];
                        R2=data_[y*row_increment_+(x+(idx2-idx1)*height_)*bytes_per_pixel_+2];
                        A2=data_[y*row_increment_+(x+(idx2-idx1)*height_)*bytes_per_pixel_+3];

                        data_[y*row_increment_+x*bytes_per_pixel_]=B2;
                        data_[y*row_increment_+x*bytes_per_pixel_+1]=G2;
                        data_[y*row_increment_+x*bytes_per_pixel_+2]=R2;
                        data_[y*row_increment_+x*bytes_per_pixel_+3]=A2;

                        data_[y*row_increment_+(x+(idx2-idx1)*height_)*bytes_per_pixel_]=B1;
                        data_[y*row_increment_+(x+(idx2-idx1)*height_)*bytes_per_pixel_+1]=G1;
                        data_[y*row_increment_+(x+(idx2-idx1)*height_)*bytes_per_pixel_+2]=R1;
                        data_[y*row_increment_+(x+(idx2-idx1)*height_)*bytes_per_pixel_+3]=A1;
                    }
                }
            }
        }


        virtual inline void set_color_for_square(const POINT TopLeft, const POINT BottomRight, const POINT PickColor, const std::string name_of_file){

            CBitmap             tmp_bmp;
            unsigned char       R,G,B,A;
            int                 idx;

            if (((TopLeft.x>width_) || (TopLeft.y>height_)) || ((BottomRight.x>width_) || (BottomRight.y>height_))){
                cout << "Eroare: Parametrii TopLeft sau BottomRight sunt in afara imaginii !\nSetatii corespunzator parametrii !" <<endl;return;
            }
            if ((PickColor.x>width_) || (PickColor.y>height_)){
                cout << "Eroare: Parametrul PickColor este in afara imaginii !\nSetatii corespunzator parametrii !" <<endl;return;
            }

            tmp_bmp = *this;
            idx=row_increment_*PickColor.y+PickColor.x*bytes_per_pixel_;

            B = data_[idx];
            G = data_[idx+1];
            R = data_[idx+2];
            A = data_[idx+3];

            for(unsigned int y=0; y<height_; y++){
                for(unsigned int x=0; x<width_; x++){
                    idx=y*row_increment_+x*bytes_per_pixel_;
                    if (((x>=TopLeft.x) && (x<=BottomRight.x)) && ((y>=TopLeft.y) && (y<=BottomRight.y))) {
                        tmp_bmp.data_[idx] = B;
                        tmp_bmp.data_[idx+1] = G;
                        tmp_bmp.data_[idx+2] = R;
                        tmp_bmp.data_[idx+3] = A;
                    }
                }
            }
            //cout << tmp_bmp.data_.size() << endl;
            tmp_bmp.save_bitmap(name_of_file);
        }


        virtual inline void cut_image_from_other_image(const POINT TopLeft, const POINT BottomRight, const std::string name_of_file){

            unsigned char                           R,G,B,A;
            CBitmap                                 tmp_bmp;
            int                                     idx, idx1, idx2;
            std::vector<unsigned char>::iterator    it;

            if (((TopLeft.x>width_) || (TopLeft.y>height_)) || ((BottomRight.x>width_) || (BottomRight.y>height_))){
                cout << "Eroare: Parametrii TopLeft sau BottomRight sunt in afara imaginii !\nSetatii corespunzator parametrii !" <<endl;return;
            }


            tmp_bmp.width_ = BottomRight.x-TopLeft.x;
            tmp_bmp.height_ = BottomRight.y-TopLeft.y;
            tmp_bmp.bytes_per_pixel_ = bytes_per_pixel_;
            tmp_bmp.row_increment_ = tmp_bmp.width_*tmp_bmp.bytes_per_pixel_;

            for(unsigned int y=0; y<tmp_bmp.height_; y++){
                for(unsigned int x=0; x<tmp_bmp.width_; x++){
                    idx = (y+TopLeft.y)*row_increment_+(x+TopLeft.x)*bytes_per_pixel_;

                    B = data_[idx];
                    G = data_[idx+1];
                    R = data_[idx+2];
                    A = data_[idx+3];

                    tmp_bmp.data_.push_back(B);
                    tmp_bmp.data_.push_back(G);
                    tmp_bmp.data_.push_back(R);
                    tmp_bmp.data_.push_back(A);
                }
            }
            //cout << "tmp_bmp.data_.size()" << tmp_bmp.data_.size() << endl;
            tmp_bmp.save_bitmap(name_of_file);
        }


        virtual inline void insert_image_into_image(const POINT TopLeft, const CBitmap& sursa_bmp, const std::string name_of_file){


            CBitmap                                 tmp_bmp;
            unsigned char                           R,G,B,A;
            int                                     idx_s, idx_d;

            if ((TopLeft.x>width_) || (TopLeft.y>height_)){
                cout << "Eroare: Parametrii TopLeft sau BottomRight sunt in afara imaginii !\nSetatii corespunzator parametrii !" <<endl;return;
            }

            tmp_bmp = *this;

            for(unsigned int y=0; y<sursa_bmp.height_; y++){
                for(unsigned int x=0; x<sursa_bmp.width_; x++){

                    idx_s = y*sursa_bmp.row_increment_+x*sursa_bmp.bytes_per_pixel_;
                    idx_d = (y+TopLeft.y)*row_increment_+(x+TopLeft.x)*bytes_per_pixel_;

                    B = sursa_bmp.data_[idx_s];
                    G = sursa_bmp.data_[idx_s+1];
                    R = sursa_bmp.data_[idx_s+2];
                    A = sursa_bmp.data_[idx_s+3];

                    tmp_bmp.data_[idx_d] = B;
                    tmp_bmp.data_[idx_d+1] = G;
                    tmp_bmp.data_[idx_d+2] = R;
                    tmp_bmp.data_[idx_d+3] = A;
                }
            }
            //cout << "tmp_bmp.data_.size()" << tmp_bmp.data_.size() << endl;
            tmp_bmp.save_bitmap(name_of_file);
        }





        virtual inline void save_bitmap(const std::string name_of_file){

            FILE*                                       pFile;
            unsigned char                               header[54];
            unsigned int                                idx;
            std::vector<unsigned char>::iterator        it, it1, it2;
            std::vector<unsigned char>                  buffer_;

            pFile = fopen(&name_of_file.c_str()[0],"wb+");
            if(pFile == NULL){cout << "Eroare: A aparut o eroare la crearea fisierului !" << file_name_ << endl; delete this; return;}


            bfh.TypeOfFile = 0x4d42;//16973;
            bfh.LengthOfFile = 54+data_.size();
            bfh.OffsetBMP = 54;

            bih.SizeOfHeader=40;
            bih.Width=width_;
            bih.Height=height_;
            bih.NoOfPlanes=1;
            bih.NoOfBitsPerPixel=32;

            //header[0]='B';
            //header[1]='M';
            ZeroMemory(&header[0], sizeof(header));
            memcpy(&header[0], &bfh.TypeOfFile,  2);
            memcpy(&header[2], &bfh.LengthOfFile, 4);
            memcpy(&header[10], &bfh.OffsetBMP, 4);

            memcpy(&header[14], &bih.SizeOfHeader,  4);
            memcpy(&header[18], &bih.Width, 4);
            memcpy(&header[22], &bih.Height, 4);
            memcpy(&header[26], &bih.NoOfPlanes, 2);
            memcpy(&header[28], &bih.NoOfBitsPerPixel, 2);

            cout << "header[0]:"  << header[0] << endl;

            it=buffer_.begin();
            //cout << "buffer.size() before: " << buffer_.size() << endl;
            //cout << "data_.size() before: " << data_.size() << endl;
            buffer_.insert(it, header, header+54);

            /**inverseaza imaginea fata de axa OX
            //===============================================
            **/

            for(unsigned int y=1; y<=height_; y++){
                for(unsigned int x=0; x<width_; x++){
                    idx = (height_-y)*row_increment_+bytes_per_pixel_*x;
                    buffer_.push_back(data_[idx]);
                    buffer_.push_back(data_[idx+1]);
                    buffer_.push_back(data_[idx+2]);
                    buffer_.push_back(data_[idx+3]);
                }
            }
            cout << "buffer_.size() after: " << buffer_.size() << endl;
            fwrite(&buffer_[0],1,buffer_.size(),pFile);
            fclose(pFile);
        }


        CBitmap& operator += (CBitmap& image1){

            std::vector<unsigned char>::iterator    itr, itr1, itr2;

            if (height_!=image1.height_){cout << "Eroare: Cele doua imagini nu pot fi unite pentru ca nu au aceeasi inaltime !" << endl; goto LABEL_21;}
            if (bytes_per_pixel_!=image1.bytes_per_pixel_){cout << "Eroare: Cele doua imagini nu pot fi unite pentru ca nu au acelasi numar de bytes per pixel !" << endl; goto LABEL_21;}

            for (int y=0; y<height_; y++){
                itr=data_.begin()+(y+1)*row_increment_+y*image1.row_increment_;
                itr1=image1.data_.begin()+y*image1.row_increment_;
                itr2=itr1+image1.row_increment_;
                data_.insert(itr, itr1, itr2);
                //printf("itr: %d\n",itr);
            }
            width_=width_+image1.width_;
            row_increment_=width_*bytes_per_pixel_;
LABEL_21:;
        }


        /*
        inline void histogram(const color_plane color, double hist[256]) const
        {
            std::fill(hist, hist + 256, 0.0);

            for (const unsigned char* itr = (data() + offset(color)); itr < end(); itr += bytes_per_pixel_)
            {
                ++hist[(*itr)];
            }
        }
        */

        inline void getBFH(std::ostringstream&  strTag){
            strTag << "BFH:" << endl;
            sTag = "  ";
            sprintf(&sTag[0], "%u", bfh.TypeOfFile);
            strTag << "   Type = " << sTag << endl;
            strTag << "   Length of BMP File = " << bfh.LengthOfFile << endl;
            strTag << "   Offset of BMP = " << bfh.OffsetBMP << endl;
            strTag << "   data_.size() = " << data_.size() << endl;
            strTag << "   row_increment_*height_ = " << row_increment_*height_ << endl;
        }

        inline void getBIH(std::ostringstream&  strTag){
            strTag << "BIH:" << endl;
            strTag << "   Size of BIH = " << bih.SizeOfHeader<< endl;
            strTag << "   width of BMP = " << bih.Width << endl;
            strTag << "   height of BMP = " << bih.Height << endl;
            strTag << "   Planes = " << bih.NoOfPlanes << endl;
            strTag << "   Bits = " << bih.NoOfBitsPerPixel << endl;
        }


        virtual inline unsigned int get_width(void){return width_;}

        virtual inline unsigned int get_height(void){return height_;}

        virtual inline unsigned int get_size_of_data(void){return data_.size();}

        virtual inline unsigned int get_this(void){return printf("0x%x",this);}

        virtual inline unsigned int get_size_data(void){return bfh.LengthOfFile-bih.SizeOfHeader-14;}

        virtual inline void get_file_name(std::ostringstream& strTag){
            strTag << "Nume fisier = " << file_name_ << endl;
            return;
        }

        virtual inline unsigned int get_size_bfh(void){return bfh.struct_size();}

        virtual inline unsigned int get_size_bih(void){return bih.struct_size();}

        virtual inline unsigned int get_no_of_bytes_per_pixel(void){return bytes_per_pixel_;}


        virtual inline void display_header_of_file(void){
            cout << "===========================================" << endl;
            cout << "            HEADER OF FILE                 " << endl;
            cout << "===========================================" << endl;
            cout << "width_: " << width_ << endl;
            cout << "height_: " << height_ << endl;
            cout << "row_increment_: " << row_increment_ << endl;
            cout << "bytes_per_pixel_: " << bytes_per_pixel_ << endl;
            cout << "bfh.OffsetBMP: " << bfh.OffsetBMP << endl;
            cout << "bfh.LengthOfFile: " << bfh.LengthOfFile << endl<<endl;
            cout << "===========================================" << endl;
        }

        virtual inline void display_header_of_bitmap(void){
            cout << "===========================================" << endl;
            cout << "            HEADER OF BITMAP               " << endl;
            cout << "===========================================" << endl;
            cout << "width : " << width_ << endl;
            cout << "height : " << height_ << endl;
            cout << "row_increment_ : " << row_increment_ << endl;
            cout << "bytes_per_pixel_ : " << bytes_per_pixel_ << endl;
            cout << "data_.size() : " << data_.size() << endl;
            cout << "row_increment_*height_: " << row_increment_*height_ << endl;
            cout << "===========================================" << endl;
        }


    /**date private specifice clasei BMP
    //================================================================
    **/
    private:
        struct bitmap_file_header{
            unsigned short      TypeOfFile;
            unsigned int        LengthOfFile;
            unsigned short      Reserved1;
            unsigned short      Reserved2;
            unsigned int        OffsetBMP;

            unsigned int struct_size() const
            {
                 return sizeof(TypeOfFile   ) +
                        sizeof(LengthOfFile ) +
                        sizeof(Reserved1    ) +
                        sizeof(Reserved2    ) +
                        sizeof(OffsetBMP    ) ;
            }
        };

        struct bitmap_information_header{
            unsigned int        SizeOfHeader;
            unsigned int        Width;
            unsigned int        Height;
            unsigned short      NoOfPlanes;
            unsigned short      NoOfBitsPerPixel;
            unsigned int        Compression;
            unsigned int        ImageSize;
            unsigned int        H_resolution;
            unsigned int        V_resolution;
            unsigned int        Color_pallete;
            unsigned int        Color_used;

            unsigned int struct_size() const
            {
                 return sizeof(SizeOfHeader     ) +
                        sizeof(Width            ) +
                        sizeof(Height           ) +
                        sizeof(NoOfPlanes       ) +
                        sizeof(NoOfBitsPerPixel ) +
                        sizeof(Compression      ) +
                        sizeof(ImageSize        ) +
                        sizeof(H_resolution     ) +
                        sizeof(V_resolution     ) +
                        sizeof(Color_pallete    ) +
                        sizeof(Color_used       ) ;
            }
        };

        bitmap_file_header              bfh;
        bitmap_information_header       bih;
 };


 class CBitmapE: public CBitmap{

    public:

        /**constructor
        //====================================================
        **/
        CBitmapE(const unsigned int width, const unsigned int height, const COLORREF color, const unsigned char no_of_bytes_per_pixel){
            width_ = width;
            height_ = height;
            bytes_per_pixel_ = no_of_bytes_per_pixel;
            row_increment_ = width_ * bytes_per_pixel_;

            if (!((no_of_bytes_per_pixel==3) || (no_of_bytes_per_pixel==4))){
               bytes_per_pixel_ = 3;
            }

            //printf("Capacitatea vectorului %d\n",data_.capacity());
            data_.reserve(width_*height_*bytes_per_pixel_);
            //printf("Capacitatea vectorului %d\n",data_.capacity());
            for(int x=0; x<width_*height_; x++){
                if (bytes_per_pixel_==4){
                    data_.push_back((static_cast<unsigned char>(color>>24)));
                }
                data_.push_back((static_cast<unsigned char>(color>>16)));
                data_.push_back((static_cast<unsigned char>(color>>8)));
                data_.push_back((static_cast<unsigned char>(color)));
            }
            //printf("Capacitatea vectorului %d\n",data_.capacity());
            //printf("Dimensiunea vectorului %d\n",data_.size());
            //printf ("0:%x\n1:%x\n2:%x\ncolor:%x\n",data_[0],data_[1],data_[2], color);
        }

 };
