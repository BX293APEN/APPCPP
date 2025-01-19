#include "ioset.hpp"

// ボタンID
#define ID_LIST 2000
#define ID_STORE 2001    
#define ID_SHUTOP 2002
#define ID_SANSHO 2003
#define ID_CMDEXE 2004
#define ID_TIME 2005
#define ID_MSGCREATE 2006
#define ID_SEARCH 2007
#define ID_SEARCHBOX 2008
#define ID_CMDBOX 2009
#define ID_PATH 2010

// 名前
HWND check;
HWND radio0, radio1, radio2, radio3, radio4, radio5;
HWND combo;
HWND text, second, filePath, powerOption, cmdPrompt, dialogIcon, msgTit, msgTex, browserQuery, searchEngine;

IOSet io("");
class APP{
    protected:
        MSG msg;
        WNDCLASS winc;
        HWND textEdit, cmdGroup, dialogGroup, dialogSettings;
        HINSTANCE hInstance;
        HWND appBody; //HINSTANCEの下に確保

        static LRESULT CALLBACK WndProc(HWND window , UINT requestMsg , WPARAM wp , LPARAM lp) {
            int wmId = LOWORD(wp);
            int checkbox = 0;
            int msgVal, mbButton, mbIcon ,help;
            int radioval[6] = {0,0,0,0,0,0};
            int pwOptionList = -1;
            int shutsec;
            int engineList = 0;
            int iconList = 0;
            std::string mbTitle, mbText, url, fileData;
            HDC hdc;
            PAINTSTRUCT ps;
            CHAR textVal[32768], secondVal[32768], fileVal[32768], cmdVal[32768], tex[32768], tit[32768], searchVal[32768];

            switch (requestMsg) { //Windowからのメッセージを分類
                case WM_CLOSE: /* × ボタンが押された時の操作 */
                    SetWindowText(window,"終了");
                    msgVal = io.MsgBox("確認","終了しますか？",4,3,0);
                    switch (msgVal){
                        case IDYES:
                            DestroyWindow(window);
                            PostQuitMessage(0);
                            return 0;

                        default:
                            SetWindowText(window, io.title("").c_str());
                            return 0;
                    }
                    
                case WM_CTLCOLORSTATIC: /*コントロールスタティックが返ってくる場所(グループボックス)の設定*/
                    SetBkMode((HDC)wp,TRANSPARENT); //背景を透明にして綺麗にする
                    if(powerOption ==(HWND)lp){
                        SetTextColor((HDC)wp,RGB(255,0,0));//テキスト
                    }
                    return (LRESULT)CreateSolidBrush( RGB( 234, 234, 234 ) ); //背景色

                case WM_LBUTTONDOWN: /*左クリック押したときの操作*/
                    SetWindowText(window,"左クリック");
                    break;

                /*左クリック押したときの操作*/
                case WM_RBUTTONDOWN:
                    SetWindowText(window,"右クリック");
                    break;

                /*フォーカスを外したときの操作*/
                case EN_KILLFOCUS:
                    SetWindowText(window, io.title("").c_str());
                    break;

                /*Windowに描画する場所の設定*/
                case WM_PAINT:
                    hdc = BeginPaint(window , &ps);

                    /* テキスト生成 */
                    SetBkColor(hdc , RGB(234 , 234 , 234));
                    SetTextColor(hdc , RGB(0 , 0 , 0));
                    TextOut(hdc , 75 , 110 , TEXT("秒") , lstrlen(TEXT("秒")));

                    SetBkColor(hdc , RGB(0 , 0 , 0));
                    SetTextColor(hdc , RGB(0 , 255 , 0));
                    TextOut(hdc , 280 , 75 , std::string(io.pwd() + " >   ").c_str() ,std::string(io.pwd() + " >   ").size());
                    //テキスト追加


                    EndPaint(window , &ps);/* テキスト生成終了 */
                    break;

                case WM_COMMAND:{ /*ボタン押したときの操作*/
                    // 選択された所を調べる
                    switch (wmId){
                        case 1:
                            SetWindowText(window,"テキストを入力");
                            break;

                        case ID_PATH:
                            SetWindowText(window,"参照したいファイルのパスの入力");
                            break;

                        case ID_SEARCHBOX:
                            SetWindowText(window,"検索ボックス");
                            break;

                        case ID_LIST:
                            SetWindowText(window,"リストから選択");
                            break;

                        case ID_CMDBOX:
                            SetWindowText(window,"コマンド入力");
                            break;

                        case ID_TIME:
                            SetWindowText(window, "実行するまでの時間(秒数)を入力");
                            break;

                        case ID_SHUTOP:
                            SetWindowText(window,"実行");
                            pwOptionList = SendMessage(combo, CB_GETCURSEL, 0, 0);
                            GetWindowText(second , secondVal , 32768);
                            shutsec = atoi(secondVal);
                            switch(pwOptionList){
                                case 0:
                                    DestroyWindow(window) ;
                                    PostQuitMessage(0);
                                    io.shutdown("logoff",shutsec);
                                    return 0;
                                case 1:
                                    DestroyWindow(window) ;
                                    PostQuitMessage(0);
                                    io.shutdown("shutdown",shutsec);
                                    return 0;
                                case 2:
                                    DestroyWindow(window) ;
                                    PostQuitMessage(0);
                                    io.shutdown("reboot",shutsec);
                                    return 0;
                            }
                            SetWindowText(window,"実行出来ません");
                            break;

                        case ID_STORE:
                            SetWindowText(window,"変更の保存");
                            msgVal = io.MsgBox("確認","内容を保存しますか？",4,3,0);
                            switch (msgVal){
                                case IDYES:
                                    GetWindowText(text , textVal , 32768);
                                    GetWindowText(filePath , fileVal , 32768);
                                    io.write_file(std::string(fileVal), std::string(textVal), "w");
                            }
                            break;

                        case ID_SANSHO:
                            SetWindowText(window,"参照");
                            GetWindowText(filePath , fileVal , 32768);
                            fileData = io.read_file(std::string(fileVal));
                            SetWindowText(text, fileData.c_str());
                            break;

                        case ID_CMDEXE:
                            SetWindowText(window,"コマンドプロンプト実行");
                            GetWindowText(cmdPrompt, cmdVal, 32768);
                            io.cmd(std::string(cmdVal));/* 実行 */
                            break;

                        case ID_MSGCREATE:
                            SetWindowText(window,"メッセージボックス生成");

                            GetWindowText(msgTex , tex , 32768);
                            GetWindowText(msgTit , tit , 32768);

                            radioval[0] = SendMessage(radio0, BM_GETCHECK, 0, 0);
                            radioval[1] = SendMessage(radio1, BM_GETCHECK, 0, 0);
                            radioval[2] = SendMessage(radio2, BM_GETCHECK, 0, 0);
                            radioval[3] = SendMessage(radio3, BM_GETCHECK, 0, 0);
                            radioval[4] = SendMessage(radio4, BM_GETCHECK, 0, 0);
                            radioval[5] = SendMessage(radio5, BM_GETCHECK, 0, 0);

                            checkbox = SendMessage(check, BM_GETCHECK, 0, 0);
                            iconList = SendMessage(dialogIcon, CB_GETCURSEL, 0, 0);

                            if(radioval[0] == 1)        mbButton = MB_OK;
                            else if(radioval[1] == 1)   mbButton = MB_OKCANCEL;
                            else if(radioval[2] == 1)   mbButton = MB_YESNO;
                            else if(radioval[3] == 1)   mbButton = MB_YESNOCANCEL;
                            else if(radioval[4] == 1)   mbButton = MB_ABORTRETRYIGNORE;
                            else if(radioval[5] == 1)   mbButton = MB_RETRYCANCEL;

                            mbIcon  = iconList;
                            help    = checkbox;
                            mbTitle = std::string(tit);
                            mbText  = std::string(tex);

                            io.MsgBox(mbTitle, mbText, mbButton, mbIcon, help); 
                            break;

                        case ID_SEARCH:
                            SetWindowText(window,"検索");
                            engineList = SendMessage(searchEngine, CB_GETCURSEL, 0, 0);
                            switch(engineList){
                                case 1:
                                    url = "youtube.co.jp/";
                                    break;
                                case 2:
                                    url = "bing.com/";
                                    break;
                                case 3:
                                    url = "twitter.com/";
                                    break;
                                default:
                                    url = "google.co.jp/";
                                    break;
                            }
                            GetWindowText(browserQuery , searchVal , 32768);
                            url = "start www."+ url +"search?q=" + std::string(searchVal);
                            io.cmd(url);
                            break;

                        default:
                            SetWindowText(window,io.title("").c_str());
                    }

                }
            }
            return DefWindowProc(window, requestMsg, wp, lp);
        }

        void ui_setting(){
            this->appBody = CreateWindow( /* Window生成 */
                "WINDOW",
                TEXT(io.title("").c_str()) ,
                (WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX) | WS_VISIBLE ,
                100 , 10 ,970 , 610 ,
                NULL , NULL , this->hInstance , NULL
            );
            
            /* リストボックス */
            searchEngine = CreateWindow(
                "COMBOBOX",
                NULL, 
                WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST , 
                20 , 10 , 150 , 150 , 
                this->appBody , (HMENU)ID_LIST ,this->hInstance, NULL
            );
            // リストボックスの選択肢
            SendMessage(searchEngine , CB_ADDSTRING , 0 , (LPARAM)TEXT("Google"));
            SendMessage(searchEngine , CB_ADDSTRING , 0 , (LPARAM)TEXT("YouTube"));
            SendMessage(searchEngine , CB_ADDSTRING , 0 , (LPARAM)TEXT("Bing"));
            SendMessage(searchEngine , CB_ADDSTRING , 0 , (LPARAM)TEXT("Twitter"));


            /* テキストエリア生成 */
            browserQuery = CreateWindow(
                "EDIT",
                NULL,
                WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_LEFT | ES_AUTOHSCROLL | WS_HSCROLL,
                180,10,620,40,
                this->appBody,(HMENU)ID_SEARCHBOX,this->hInstance,NULL
            );

            SetWindowText(browserQuery, TEXT("検索")); /* テキストエリアに文字表示 */

            /* ボタン生成 */
            CreateWindow(
                "BUTTON",
                TEXT("search"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
                820 , 10 , 100 , 30 ,
                this->appBody , (HMENU)ID_SEARCH , this->hInstance , NULL
            );    

            /* グループボックス生成 */
            powerOption = CreateWindow(
                "BUTTON" , "電源オプション" ,
                WS_CHILD | WS_VISIBLE | BS_GROUPBOX ,
                20 , 50 , 240 , 120 ,
                this->appBody , NULL , this->hInstance , NULL
            );


            /* リストボックス */
            combo = CreateWindow(
                "COMBOBOX",
                NULL, 
                WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST , 
                40 , 75 , 200 , 200 , 
                this->appBody , (HMENU)ID_LIST ,this->hInstance, NULL
            );
            // リストボックスの選択肢
            SendMessage(combo , CB_ADDSTRING , 0 , (LPARAM)TEXT("ログオフ"));
            SendMessage(combo , CB_ADDSTRING , 0 , (LPARAM)TEXT("シャットダウン"));
            SendMessage(combo , CB_ADDSTRING , 0 , (LPARAM)TEXT("再起動"));

            /* テキストエリア生成 */
            second = CreateWindow(
                "EDIT",
                NULL,
                WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_RIGHT | ES_NUMBER,
                40,110,30,20,
                this->appBody,(HMENU)ID_TIME,this->hInstance,NULL
            );

            /* テキストエリアに文字表示 */
            SetWindowText(second, TEXT("30"));

            /* ボタン生成 */
            CreateWindow(
                "BUTTON",
                TEXT("決定"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
                140 , 130 , 100 , 30 ,
                this->appBody , (HMENU)ID_SHUTOP , this->hInstance , NULL
            );

            /* グループボックス生成 */
            this->textEdit = CreateWindow(
                "BUTTON" , "テキスト編集" ,
                WS_CHILD | WS_VISIBLE | BS_GROUPBOX ,
                20 , 190 , 500 , 380 ,
                this->appBody , NULL , this->hInstance , NULL
            );

            /* テキストエリア生成 */
            filePath = CreateWindow(
                "EDIT",
                NULL,
                WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_LEFT | ES_AUTOHSCROLL | WS_HSCROLL,
                40,215,460,40,
                this->appBody,(HMENU)ID_PATH,this->hInstance,NULL
            );

            /* テキストエリアに文字表示 */
            SetWindowText(filePath, io.pwd().c_str());

            /* ボタン生成 */
            CreateWindow(
                "BUTTON",
                TEXT("参照"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
                370 , 260 , 100 , 30 ,
                this->appBody , (HMENU)ID_SANSHO , this->hInstance , NULL
            );


            /* テキストエリア生成 */
            text = CreateWindow(
                "EDIT",
                NULL,
                WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_LEFT | ES_AUTOVSCROLL | WS_VSCROLL,
                40,310,400,200,
                this->appBody,(HMENU)1,this->hInstance,NULL
            );

            /* テキストエリアに文字表示 */
            SetWindowText(text, TEXT("テキスト"));


            /* ボタン生成 */
            CreateWindow(
                "BUTTON",
                TEXT("保存"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
                370 , 520 , 100 , 30 ,
                this->appBody , (HMENU)ID_STORE , this->hInstance , NULL
            );

            /* グループボックス生成 */
            this->cmdGroup = CreateWindow(
                "BUTTON" , "コマンドプロンプト" ,
                WS_CHILD | WS_VISIBLE | BS_GROUPBOX ,
                270 , 50 , 670 , 120 ,
                this->appBody , NULL , this->hInstance , NULL
            );

            /* テキストエリア生成 */
            cmdPrompt = CreateWindow(
                "EDIT",
                NULL,
                WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_LEFT | ES_AUTOHSCROLL | WS_HSCROLL,
                280 + 8*(std::string(io.pwd() + " >   ").size()), 75, 910-(280 + 8*(std::string(io.pwd() + " >   ").size()) - 4), 40,
                this->appBody,(HMENU)ID_CMDBOX,this->hInstance,NULL
            );

            /* ボタン生成 */
            CreateWindow(
                "BUTTON",
                TEXT("実行"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
                720 , 125 , 100 , 30 ,
                this->appBody , (HMENU)ID_CMDEXE , this->hInstance , NULL
            );

            /* グループボックス生成 */
            this->dialogGroup = CreateWindow(
                "BUTTON" , "メッセージボックス作成" ,
                WS_CHILD | WS_VISIBLE | BS_GROUPBOX ,
                535 , 190 , 405 , 380 ,
                this->appBody , NULL , this->hInstance , NULL
            );

            /* グループボックス生成 */
            this->dialogSettings = CreateWindow(
                "BUTTON" , "設定" ,
                WS_CHILD | WS_VISIBLE | BS_GROUPBOX  ,
                545 , 213 , 390 , 145 ,
                this->appBody , NULL , this->hInstance , NULL
            );

            /* リストボックス */
            dialogIcon = CreateWindow(
                "COMBOBOX",
                NULL, 
                WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST , 
                560 , 240 , 130 , 130 , 
                this->appBody , (HMENU)ID_LIST ,this->hInstance, NULL
            );
            // リストボックスの選択肢
            SendMessage(dialogIcon , CB_ADDSTRING , 0 , (LPARAM)TEXT("アイコンなし"));
            SendMessage(dialogIcon , CB_ADDSTRING , 0 , (LPARAM)TEXT("注意アイコン"));
            SendMessage(dialogIcon , CB_ADDSTRING , 0 , (LPARAM)TEXT("情報アイコン"));
            SendMessage(dialogIcon , CB_ADDSTRING , 0 , (LPARAM)TEXT("質問アイコン"));
            SendMessage(dialogIcon , CB_ADDSTRING , 0 , (LPARAM)TEXT("警告アイコン"));


            /* ラジオボタン生成 */
            radio0 = CreateWindow(
                "BUTTON",
                TEXT("[OK]") ,
                WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
                560 , 265 , 160 , 30 ,
                this->appBody , NULL ,this->hInstance , NULL
            );

            /* ラジオボタン生成 */
            radio1 = CreateWindow(
                "BUTTON",
                TEXT("[OK][キャンセル]") ,
                WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON ,
                560 , 295 , 160 , 30 ,
                this->appBody , NULL ,this->hInstance , NULL
            );

            /* ラジオボタン生成 */
            radio2 = CreateWindow(
                "BUTTON",
                TEXT("[はい][いいえ]") ,
                WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON ,
                560 , 325 , 160 , 30 ,
                this->appBody , NULL ,this->hInstance , NULL
            );


            /* ラジオボタン生成 */
            radio3 = CreateWindow(
                "BUTTON",
                TEXT("[はい][いいえ][キャンセル]") ,
                WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON ,
                720 , 235 , 210 , 30 ,
                this->appBody , NULL ,this->hInstance , NULL
            );

            /* ラジオボタン生成 */
            radio4 = CreateWindow(
                "BUTTON",
                TEXT("[中止][再試行][無視]") ,
                WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON ,
                720 , 265 , 200 , 30 ,
                this->appBody , NULL ,this->hInstance , NULL
            );

            /* ラジオボタン生成 */
            radio5 = CreateWindow(
                "BUTTON",
                TEXT("[再試行][キャンセル]") ,
                WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON ,
                720 , 295 , 200 , 30 ,
                this->appBody , NULL ,this->hInstance , NULL
            );

            /* チェックボックス生成 */
            check = CreateWindow(
                "BUTTON" ,
                TEXT("[ヘルプ]") ,
                WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX ,
                720 , 325 , 200 , 30 ,
                this->appBody , NULL , this->hInstance , NULL
            );

            /* テキストエリア生成 */
            msgTit = CreateWindow(
                "EDIT",
                NULL,
                WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_LEFT | ES_AUTOHSCROLL | WS_HSCROLL,
                550,365,350,40,
                this->appBody,(HMENU)1,this->hInstance,NULL
            );

            /* テキストエリアに文字表示 */
            SetWindowText(msgTit, TEXT("タイトル入力"));

            /* テキストエリア生成 */
            msgTex = CreateWindow(
                "EDIT",
                NULL,
                WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_LEFT | ES_AUTOVSCROLL | WS_VSCROLL,
                550,415,270,140,
                this->appBody,(HMENU)1,this->hInstance,NULL
            );
            /* テキストエリアに文字表示 */
            SetWindowText(msgTex, TEXT("テキスト入力"));

            /* ボタン生成 */
            CreateWindow(
                "BUTTON",
                TEXT("作成"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
                830 , 525 , 100 , 30 ,
                this->appBody , (HMENU)ID_MSGCREATE , this->hInstance , NULL
            );

        }
        
        void __init__(std::string appTitle){ /* 初期設定 */
            io.title(appTitle);
            io.cmd("color 0A");
        }

    public:
        void loop(){
            while(GetMessage(&this->msg , NULL , 0 , 0)){
                TranslateMessage(&this->msg);
                DispatchMessage(&this->msg);
            }
        }
        int end(){ // 閉じた後の処理
            io.cmd("cls");
            io.print("終了しています");
            io.cmd("color 07");
            UnregisterClass("WINDOW",this->hInstance);
            Sleep(10);
            return this->msg.wParam;
        }
        APP(std::string appTitle = "アプリ"){
            this->winc.style                = CS_HREDRAW | CS_VREDRAW;
            this->winc.lpfnWndProc          = this->WndProc;
            this->winc.cbClsExtra           = 0;
            this->winc.cbWndExtra           = 0;
            this->winc.hInstance            = this->hInstance;
            this->winc.hIcon                = LoadIcon(NULL , IDI_APPLICATION);
            this->winc.hCursor              = LoadCursor(NULL , IDC_ARROW);
            this->winc.hbrBackground        = (HBRUSH)CreateSolidBrush( RGB( 234, 234, 234 ) );
            this->winc.lpszMenuName         = NULL;
            this->winc.lpszClassName        = "WINDOW";

            if (!RegisterClass(&this->winc)){
                return;
            }
            this->__init__(appTitle);
            this->ui_setting();
            
        }
};

int main(int argc, char *argv[]){
    APP app("PEN");
    app.loop();
    app.end();
    return 0;
}