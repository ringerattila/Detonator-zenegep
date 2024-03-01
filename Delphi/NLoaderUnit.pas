unit NLoaderUnit;
// Módosotáskor a Loaderver értékét át kell írni!!!
interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms, 
  Dialogs, StdCtrls, ExtCtrls, ShellAPI, MD5, display, Startup_unit, Serial, TlHelp32, Det_sql,
  det_ftp, det_const, det_skin, jpeg;

type
  TLoaderForm = class(TForm)
    Timer1: TTimer;
    Label1: TLabel;
    LoaderImage: TImage;
    LabelFirstRow: TLabel;
    LabelSecondRow: TLabel;
    LabelThirdRow: TLabel;
    LabelFifthRow: TLabel;
    Label2: TLabel;
    procedure FormCreate(Sender: TObject);
    procedure OnTimer1(Sender: TObject);
    procedure OnTimer_2(Sender: TObject);
    procedure DoQuit(Sender: TObject);

//    procedure FormKeyPress(Sender: TObject; var Key: Char);

  private
    { Private declarations }

  public
    Constructor Create(Owner: TComponent); OverRide;
 //   property FTPGetComplete : Boolean read FFTPGetComplete;
    { Public declarations }
  end;


Const NewLoaderVer :string = 'v5.26L';
  RestartFile = 'C:\restart.tmp'; // ha ez a file létezik, újra kell
//  indítani a main.exe-t, ha nem, lépjen ki ez a program is.

var
  LoaderForm: TLoaderForm;
  KellBillentyu: Boolean; // Kell-e a foprogramba billentyu-vezerles
  Szamol : Integer;
  Elso : Boolean;
  NetSpd : String;  // Internet letoltési sebesség string formában
  Jogos : String;  // SQL szerverről olvasott jogosultság
  MyFile : String;
  SuccExec : Integer;
  VanProc : Boolean;
  RealMBit : Real;


// procedure VarInit;
procedure DisplayInit;
procedure CheckMainFile;
Procedure HandleParams; // Kezeli a parancssori paramétereket
procedure LoaderDisplayStatus(RE:Byte; MSGType: Byte; MyText: String);
Function WaitForInternet(MyServerConnSQL: TServerConnSQL): Boolean;
//Function processExists(exeFileName: string): Boolean;
Procedure  Set_Display_Form;
Procedure  LoaderWork;

implementation

{$R *.dfm}


{Main create}
Constructor TLoaderForm.Create(Owner: TComponent);
Begin
  Inherited Create(Owner);
  LoaderForm.Top:=0;
  LoaderForm.Left:=0;
  LoaderForm.Width:= 1024;
  LoaderForm.Height:= 768;
  LoaderForm.Visible:= True;
  LoaderForm.DoubleBuffered := True;
//  Mainbg.Create(Owner);
//   Mainbg.Picture.LoadFromFile('main_bg4');
//  FFTPGetComplete := False;
End;


procedure TLoaderForm.FormCreate(Sender: TObject);
// FailIfExists : Boolean;

begin
LoaderForm.Caption := 'NLoader running';
// MainWindow := 'Form3';
     Szamol := 0;
     Label1.Caption := 'Starting main';
     Elso := True;  // Ha igen, még nem indult el a main

 // Timer beallitasok
  With Timer1 do Begin
    Enabled := False;
    Interval := 2000; // Ennyi idokozonkent probalja meg beolvasni a file-t
    Enabled := False; // DEBUG !!!!, amúgy True
  End;
  LoaderForm.Label2.Caption:= NewLoaderVer;
 end;

procedure TLoaderForm.OnTimer1(Sender: TObject);
begin
  LoaderForm.Label2.Caption:= 'OnTimer1 '  + NewLoaderVer;
end;

procedure TLoaderForm.OnTimer_2(Sender: TObject);
var
MYFile, Keres : String;
H: HWnd; // Handle meghivasa
MD5Hash, MD5HashBackup : string;

begin
  Timer1.Enabled := False;  // letiltjuk a további időzítő megszakítást
Inc(Szamol);

 Keres := 'Display_Form';
      H := FindWindow(nil, PChar(Keres));
      If (H = 0) and (Elso = False) then begin
      // már futott, de kilépett
      If FileExists(RestartFile) then begin

    MD5Hash := 'xx'; // csak legyen valami alapértéke
    MD5HashBackup := 'yy'; // csak legyen valami más alapértéke

  // main.exe file-ok összehasonlítása
  If FileExists(AlapDir + 'main.exe') Then
    MD5Hash := CalculateMD5(AlapDir + 'main.exe');

 If FileExists(AlapDir + 'BACKUP\main.exe') Then
    MD5HashBackup := CalculateMD5(AlapDir + 'BACKUP\main.exe');

  If MD5Hash <> MD5HashBackup then begin
       If FileExists(AlapDir + 'main.exe') Then DeleteFile(AlapDir + 'main.exe');
       CopyFile(PChar(AlapDir+'BACKUP\main.exe'),PChar(AlapDir+'main.exe'), True);
       Label1.Caption := 'Copying files';
       End; // end of Hash<>Hash

        Elso := True; // induljon újra
        DeleteFile(RestartFile);
        Label1.Caption := 'Have to restart.';
        end else begin
          // nincs restart.tmp, ki lehet lépni
        sleep(4000);
    //    LoaderForm.Close; // már nem kell
        Application.Terminate;
          end;
      end;

      If H <> 0 Then begin
        Label1.Caption := 'Program has started.';
        end;

  If ELso then begin
      MyFile := AlapDir + 'main.exe';
      Elso := False;  // már elindítottuk a programot
      Label1.Caption := 'Started';
      end;
    Timer1.Enabled := True;  // újra mehet a megszakítás
end;



procedure TLoaderForm.DoQuit(Sender: TObject);
var
  KES: Integer;

begin

  GepLeall := Now;

  for KES := 1 to 300 do
    Sleep(1); // kesleletetes, hogy ne villanjon be a winfos kepe
  ShowCursor(True); // kurzor visszakapcsolasa
  Close;
//  Application.Terminate;
end;


// Ha megnyomják a K betűt, miközben ez a program aktív, lépjen ki.

// Képernyőre rajzolás inicializálása
procedure DisplayInit;
begin
  LoaderForm.FormStyle := fsNormal; // átírtam normálra, hogy lehessen debuggolni<<<<
  LoaderForm.LabelFirstRow.Caption:= 'STARTING SYSTEM';
  LoaderForm.Show;
  LoaderForm.Update;
end;

 // Parancssori paraméterek
 // -version - kiírja a képrenyőre a verziószámot, aztán kilép
 // -kbc - engedélyezi a PC billentyűzet használatát
 // -help - Kiírja a használható parancssori paraméterek listáját
 // -cvf - Creat Version File - lemásolja a .exe file-t és a nevéhez illeszti a verziószámot
// Ha volt, akkor kezeli és kilép a programból

Procedure HandleParams;
var MyPar1 : String;
NewName : string;
VerString : String;
MD5Hash : string;

begin
 If (ParamCount > 0) then Begin
      MyPar1 :=   ParamStr(1);

    // Ha a paraméter a -version
    If MyPar1 = '-version' Then Begin
      ShowMessage('nLOADER '+NewLoaderVer);
      ExitProcess(0);
    End; // end of -version

   // Ha a paraméter a -kbc
    KellBillentyu := False;
   If MyPar1 = '-kbc' then KellBillentyu := True;

    // Ha a paraméter a -cvf
    If MyPar1 = '-cvf' Then Begin
        If FileExists(AlapDir + 'nloader.exe') Then
                begin
                VerString := NewLoaderVer[2] + NewLoaderVer[4] + NewLoaderVer[5];
                NewName := Alapdir + 'nloader' + VerString + '.exe';
                MD5Hash := CalculateMD5(AlapDir + 'nloader.exe');
                AssignFile(OutFile, AlapDir + 'nloader' + VerString + '.md5');
                ReWrite(OutFile);
                Write(OutFile,MD5Hash + ' *nloader' + VerString + '.exe');
                CloseFile(OutFile);
                CopyFile(PChar(AlapDir + 'nloader.exe'), PChar(NewName), False);
                ShowMessage('Creating Version File'+#13#10+
                'nLoader'+VerString + '.exe'+#13#10+
                'nloader'+VerString + '.md5');
        end;
      ExitProcess(0);
      End; // end of -vcf

    // Ha a paraméter a -help
    If MyPar1 = '-help' Then Begin
      ShowMessage('-kbc - Enable PC keyboard'+#13#10+
      '-version - Displays the version number'+#13#10+
      '-cvf - Create Version File'+#13#10+
      '-help - Displays this list');
      ExitProcess(0);
      End; // end of -help

  end; // end of paramcount

end; // end of Function


// Beállítja a Display_Form Loader-ben nem használt elemeit
Procedure  Set_Display_Form;
begin
  Display_Form.visible := False; //  Display_Form.Initialize;
  Display_Form.SystemTimer.Enabled := False;
  Display_Form.RepeatTimer.Enabled := False;
  Display_Form.ReleaseTimer.Enabled := False; // parasztosan - ne legyen hosszu nyomas ilyenkor
end;


Procedure  LoaderWork;

var IsNewSoftware : boolean;
  VoltHiba : Boolean;
  ftpTries : Integer;
  sorosPort : boolean;
  curDir : String;
  pathToStart : String;

Begin
  NetMachine := False;  // A detonator.ini-ból olvassuk be az értéket
  DisplayInit;
  Sleep(200);
  VarInit;
  RealMbit := 0; // Inicializálás;
  VoltHiba := ReadIniFile;  // Beolvassa a detonator.ini tartalmát
  if VoltHiba then
    begin
    LoaderDisplayStatus(2, ERRMSG, 'INI file error. ');
    sleep(4000);
    NetMachine := False;
    end;

// Ha az .ini-ból beolvasott Loader verziószám eltér,
// akkor már egy újabb verzió fut, és ki kell írni az ini file-ba.
// LoaderVer - ini-ből beolvasott Loader verzió
// SWLoader - programban tárolt verziószám
  If LoaderVer <> NewLoaderVer then
      SaveLoaderVersionToIniFile(NewLoaderVer);
  LogServCode := TaroltSCode;  // A file-ból olvasott szervizkod, lehet, hogy a HW mást ad majd

  // A hardvertől bekérjük a szervizkódot
  LoaderDisplayStatus(2, INFOMSG, 'Checking serial port ');
  sorosPort := ReadHW; // Hardware megszólítása, servcode és egyebek beolvasása
  if not sorosPort Then
    begin
    if not SerialPortFound then LoaderDisplayStatus(2, ERRMSG, 'Serial Port not found')
      else if not vanHWInit then LoaderDisplayStatus(2, ERRMSG, 'Hardware not found');
      NetMachine := False;
    end;

  If NetMachine then
    begin
    LoaderDisplayStatus(3, STATMSG, 'Port Nr: ' + IntToStr(Display_Form.Serial1.Port) + ' - ID: ' + IntToStr(ID_code));
    LongBootSleep(1000);
    LoaderDisplayStatus(2, INFOMSG, 'Reading setup');
    DecodeSQLServer(MyEncServerConnSQL);
  // 1. fázis - Van internet kapcsolat?
  // Abból indulunk ki, hogy van internet kapcsolat.
 // Ha nincs SQL csatlakozás, vagy nincs FTP csatlakozás, akkor False-ra állítjuk.
 // Internet kapcsolat ellenőrzése
    Van_Net := True;
  // Induláskor még nem biztos, hogy felállt a wi-fi, itt várjuk meg.
    LoaderDisplayStatus(2, INFOMSG, 'Waiting for internet connection');
    LoaderDisplayStatus(3, STATMSG, '');
    LogWrite(LogSystem, msWaitingForInternet);

    If WaitForInternet(MyServerConnSQL) then
      begin
      LoaderDisplayStatus(2, OKMSG, 'Connected to server');
      end else
      begin  // nincs internet kapcsolat, nem lesz netes gép
      LogWrite(LogSystem, msNoInternet);
      LoaderDisplayStatus(2, ERRMSG, 'No Internet Connection');
      LoaderDisplayStatus(5, STATMSG, 'Harddisk based jukebox');
      NetMachine := False;
      end;
  end; // end of if netmachine

  // 1. fázis INI file alapján netes gép-e?
  If NetMachine then
    begin
    LoaderDisplayStatus(5, STATMSG, 'Internet based jukebox');
    LongBootSleep(1000);
    LoaderDisplayStatus(3, STATMSG, 'DT: ' + DetVer + '  Main: ' + ProgVer);
    LongBootSleep(1000);
    SQLDisConnect;
    end;  // end of 1-2. fázis

  LongBootSleep(1000);

  If NetMachine then
    begin
    // 3. fázis - Van jogosultság?
    LoaderDisplayStatus(2, INFOMSG, 'Check permission');
    Jogos := ReadSQLStatus(ID_Code); // A MySQL szerverről beolvassuk a státuszt
    If Jogos <> 'Enabled' then
      begin
      LoaderDisplayStatus(3, ERRMSG, 'Status: Not enabled');
      NetMachine := False;   // nem lesz netes gép
      end else
      begin
      LogWrite(LogSystem, msEnabled);
      LoaderDisplayStatus(2, INFOMSG, dbGyarszam + ' - ' + dbNickname);
      LoaderDisplayStatus(3, OKMSG, 'Status: ENABLED');
      LongBootSleep(2000);  // hogy látni lehessen a kiirást
      ReadSQLSettings(ID_Code); // A MySQL szerverről beolvassuk a setup-ot.
      SetLogSettingsFromServer(ID_Code);
      LoaderDisplayStatus(3, OKMSG, 'SQL OK');
      end;
    end; // end of 3. fázis

  Sleep(100);
  If NetMachine then
    begin
    LogWrite(LogSystem, msConnectingFtp);
    // 4. fázis - Letöltési sebesség mérése
    If not ConnectFTP(MyServerConnFTP) then
      begin   // Nem tud kapcsolódni
      ftpTries := 1;
      LoaderDisplayStatus(2, ERRMSG, 'Conecting FTP error');
      repeat
        LoaderDisplayStatus(3, ERRMSG, 'Retry: ' + IntToStr(ftpTries) + '/5');
        sleep(500);
        Inc(ftpTries);
      until ConnectFTP(MyServerConnFTP) or (ftpTries = 6);

      if not ftpConnected then NetMachine := false;
      sleep(2000); // hadd lássák a feliratot
    end;  // Ha nem tud kapcsolódni, a NetMachine False lesz
    // Kapcsolódott, töröljük a hibaüzenetet
  LoaderDisplayStatus(2, INFOMSG, '');
  LoaderDisplayStatus(3, INFOMSG, '');
  end;

  If NetMachine then
    begin  // Kapcsolódott az FTP-hez
    LogWrite(LogSystem, msMeasureNetSpeed);
    LoaderDisplayStatus(2, INFOMSG, 'Measuring Internet speed');
    RealMbit := Netspeed;
    NetSpd := FloatToStrF(RealMBit, ffFixed, 3, 2);
    NetSpd := 'Download speed: ' + NetSpd  + ' Mbit/s';
    LoaderDisplayStatus(3, STATMSG, NetSpd);
    LogWrite(LogSystem, NetSpd);
    If RealMbit < NetSettings.NetMinSpeed then
      begin
      LoaderDisplayStatus(3, ERRMSG, 'Slow internet connection');
      NetMachine := False;
      end;
    end; // end of 4. fázis, if NetMachine

  If NetMachine then
    begin
    // Megnézzük, hogy Main.exe-nek van-e újabb verziója
    IsNewSoftWare := False;
    LoaderDisplayStatus(2, INFOMSG, 'Searching for new software');
    LongBootSleep(300);
    if NVFProgVer <> VFProgVer then
      begin
      LoaderDisplayStatus(3, STATMSG, 'Downloading new software');
      MyFile := 'main';
      IsNewSoftWare := DownLoadNewSoftware(MyFile, NVFProgVer);
      If IsNewSoftware then  LoaderDisplayStatus(3, OKMSG, 'main.exe is updated')
        else  LoaderDisplayStatus(3, ERRMSG, 'main.exe download error');
      end
      // nem tér el a verzió, nem kell letölteni
      else  LoaderDisplayStatus(3, STATMSG, 'main.exe is up to date');
    CheckMainFile;
  end; // end of if NetMachine

  If NetMachine then
    begin
      // internetes program elindítása
    LogWrite(LogSystem, msStartingMain);
    Display_Form.Serial1.Active := False;  // Soros portot bezárjuk
    MyFile := AlapDir + '\main.exe';
//    MyFile := AlapDir + '\titkosito.exe';     //csak teszt
    SuccExec := ShellExecute(0,'open',PChar(MyFile),'', nil, SW_SHOWNORMAL);
    end;  // end of if NetMachine

     // Ha nem netes gép, elindítjuk a régi programot, ha létezik
  If not NetMachine then begin
 //   shutdownDatabase; //  databaseShutdown;
    curDir := GetCurrentDir;
    if (length(OldDir) > 2) then  begin   // régi verzió indítása
      LoaderDisplayStatus(5, STATMSG, 'Starting HARDDISK based jukebox');
      LongBootSleep(3000); // Várunk, hogy lássák a feliratot

      ChDir(oldDir);
      curDir := GetCurrentDir;
      SetCurrentDir(oldDir);
//      MyFile := OldDir + '\detonator.exe';
      MyFile := OldDir + '\detonator.exe';
      pathToStart := oldDir + '\detonator.exe';
      SuccExec := ShellExecute(0,'open', PChar(MyFile),'', nil, SW_SHOWNORMAL);
    end
    else  begin // nincs régi verzió, kikapcs.
      LoaderDisplayStatus(5, STATMSG, 'Start failed. Stopping jukebox');
      sleep(2000);  // Legyen idő elolvasni
      H_MESS[1] := H_STX;
      H_MESS[2] := Ord(H_SHDOWNSEND);
      H_MESS[3] := H_ETX;
      SendSerial;
    end;

  end;  // end of if not netmachine

  // Kilépés az nLoader-ből. Ha van régi verzió, ha nincs.
  Display_Form.Serial1.Active := False;  // Soros portot bezárjuk
  sleep(400);  // A soros utasítás menjen ki
  LoaderForm.close; // mshutdown volt
  Display_Form.close;

//      Application.Terminate;
//      Exit;

end;  // end of LoaderWork

// Összehasonlitja  a/Backup könyvtárban lévő main.exe-t a
// program könyvtárban lévő main.exe-vel, ha nem egyezik, akkor kimásolja
// a /backup könyvtárból a program könyvtárba.

procedure CheckMainFile;
var
 MD5Hash, MD5HashBackup : string;

begin

  MD5Hash := 'xx'; // csak legyen valami alapértéke
  MD5HashBackup := 'yy'; // csak legyen valami más alapértéke

  // main.exe file-ok összehasonlítása
  If FileExists(AlapDir + 'main.exe') Then
    MD5Hash := CalculateMD5(AlapDir + 'main.exe');

  If FileExists(AlapDir + 'BACKUP\main.exe') Then
    MD5HashBackup := CalculateMD5(AlapDir + 'BACKUP\main.exe');

  If MD5Hash <> MD5HashBackup then
    begin
    If FileExists(AlapDir + 'main.exe') Then DeleteFile(AlapDir + 'main.exe');
    CopyFile(PChar(AlapDir+'BACKUP\main.exe'),PChar(AlapDir+'main.exe'), True);
    End; // end of Hash<>Hash
end;

procedure LoaderDisplayStatus(RE:Byte; MSGType: Byte; MyText: String);
begin

case RE of
    1: begin
       With LoaderForm.LabelFirstRow do Begin
        Color := LoaderDispBgColor[MSGType];
        Font.Color := LoaderDispFontColor[MSGType];
        Caption := MyText;
      end;
      end; // end of case 1


    2: begin
       With LoaderForm.LabelSecondRow do Begin
        Color := LoaderDispBgColor[MSGType];
        Font.Color := LoaderDispFontColor[MSGType];
        Caption := MyText;
      end;
      end; // end of case 2

   3: begin
       With LoaderForm.LabelThirdRow do Begin
        Color := LoaderDispBgColor[MSGType];
        Font.Color := LoaderDispFontColor[MSGType];
        Caption := MyText;
      end;
      end; // end of case 3

   5: begin
       With LoaderForm.LabelFifthRow do Begin
        Color := LoaderDispBgColor[MSGType];
        Font.Color := LoaderDispFontColor[MSGType];
        Caption := MyText;
      end;
      end; // end of case 3

      end; // end of case

   LoaderForm.Update;

end;


(* Ez a függvény induláskor hívódik meg, és nem biztos, hogy már felállt az
internet kapcsolat. Ezért kb. 20 másodpercig próbálkozgat, hogy van-e kapcsolat.
Ha ezután sincs, nem lesz internet alapú a zenegép. *)
Function WaitForInternet(MyServerConnSQL: TServerConnSQL): Boolean;
Var I, J : Integer;
MyHiba, Csillagok : String;

begin
  Result := False;    // Alaphelyzetben nincs csatlakozás
  J := 1;
  Csillagok := '*';
  LoaderDisplayStatus(3, INFOMSG, Csillagok);
  Repeat
    try ConnectSQL(MyServerConnSQL);

    Except
      on E : Exception do
        begin
        MyHiba := E.ClassName + ' - ' + E.Message;
        LogWrite(LogSystem, '...wait for internet');
        LogWrite(LogSystem, MyHiba);
        // end;
        If J = 1 then  LoaderDisplayStatus(3, INFOMSG, Csillagok);

        SQLDisConnect;  // szétkapcsolunk
        Sleep(1000);    // várunk
        Inc (J);
        Van_Net := False;
        NetMachine := False;
        Csillagok := '*';
        For I := 1 to J do Csillagok := Csillagok + '*';
        LoaderDisplayStatus(3, INFOMSG, Csillagok);
      end;  // end of ON
        //    if (E.ClassName = 'EZSQLException') and (Copy(E.Message, 0, 33) = 'SQL Error: Access denied for user')
    End;  // end of Except

    If DetSQL.DetConn.Connected then
      begin
      // Csatlakoztunk, ki kell jönni a ciklusból
      J := 10;  // 20 legyen
      NetMachine := True;
      Van_Net := True;
      Result := True;
      end;

    until J = 10; // 20 legyen

end; // end of Function

end.
