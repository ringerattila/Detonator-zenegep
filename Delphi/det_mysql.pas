unit det_mysql;
// Detonator zenegep MySQL rutinok
interface

uses
  SysUtils, Classes, ZAbstractDataset, ZDataset, Grids, DBGrids, IdTCPConnection, IdTCPClient,
  IdFTP, IdHTTP, jpeg, uLkJSON, IdBaseComponent, IdComponent, ComCtrls,
  ZAbstractConnection, ZConnection, DB, ZAbstractRODataset;

type

 TDetMySql = class(TComponent)
    ZQuery1: TZQuery;
    DetConn: TZConnection; // MyConn1 volt
    Procedure SetDetConn(Host, User, Pass, Database: String);

   private
 //   FFTPGetComplete : Boolean;
    { Private declarations }
  public
    Constructor Create(Owner: TComponent); OverRide;
 //   property FTPGetComplete : Boolean read FFTPGetComplete;
    { Public declarations }
  end;

  Settings2 = Record        {Beallitasok}
//    Volume : Integer;                {Hangero}
//    Credits : Integer;               {Creditek szama}
//    Szazalek : Integer;              {Counternel a szazalekertek}
//    AutoStepMin : Integer;                  {Minimum lepegetes}
//    AutoStepMax : Integer;                  {Maximum lepegetes}
// MoneyForCredit lesz    COIN : Array[1..6] of Integer;   {Adott ertek}
// getcredit lesz    CVALUE : Array[1..6] of Integer; {            = kapott CREDIT}
//    VCH : Array[1..6] of Integer;    {Ermecsatornak erteke 20/50/100 pld...}

//    RepeatLimit : Byte;              {Tobbszori lejatszas engedelyezese 0-mindig, 1-egy szamnak kell kozte lenni, stb.}
    MARADEK : Integer;               {Mennyi zseton van kimaradva}
//    NeedDemo: Boolean;               {Kell-e demozni}
//    DEMOFADE: Integer;               {Demo lekeveres hossza}
//    DEMOTOP: Integer;                {Honnan legyen demo (top mennyi) 1-10, 2-20, ... 6-mind}
//    DSTART: Integer;                 {Hany mp utan legyen demozas}
//    CUTOFF: Integer;                 {Levagas ideje mp-ben}
//    ANIM: Integer;                   {Animacio kell-e 0-nem 1..x tipusok}
//    MaxVolume: Integer;              {Maximalis hangero}
//    KARAOKE: Byte;                   {Karaoke szam erteke}
//    CreditKaraoke : Byte;
    //    VIDEO: Byte;                     {Videoklipp erteke}
//    MicLevel: Byte;                  {Mikrofon hangereje}
    {Ideiglenes valtozok az adatfileban}
//    TEMP1 : Integer;                 { Free Mode Type }
//    TEMP2 : Integer;                 {    Nincs hasznalva}
//    TEMP3 : Integer;                 {    Nincs hasznalva}
//    TEMPB1 : Boolean;                {    Nincs hasznalva}
//    TEMPB2 : Boolean;                {    Nincs hasznalva}
//    TEMPBY1 : Byte;                  {Minimum hangero}
//    TEMPBY2 : Byte;                  {Mix Mode}
//    TEMPSTR1 : String[30];           {3-asra atallt-e}
//    PENZEK: TPenzek;                 {Ermek forintkent tarolasa}
  End;


  // SQL setup altal inicializalt valtozok
    TSettings2 = Record
  //    MicLevel : Byte;        // Mikrofon hangero szintje
      MicEnabled : Boolean;  // Kell-e mikrofon albumot rajzolni
//      MaxVolume: Integer;     // A zenegepen maximalisan beallithato hangero
//      MinVolume: Integer;     // A zenegepen minimalisan beallithato hangero
//      AktVolume: Integer;     // Aktualis hangero
//      FTPServer: String;      //
//      FTPRemoteLib: String;
//      FTPUserName: String;
//      FTPPassword: String;
//      AlbumServer: String;
//      MP3Server: String;      // mp3 file-ok elerese
      SerialLog: Char;        // Soros port naplozasa D - diskre, S-server, N-no log, A-mindket log
      FTPLog: Char;
      SystemLog: Char;
      CoinLog: Char;
      PlayedLog: Char;        // Zenelejatszasok naplozasa
      AnimType: Byte;         // Animacio kell-e 0-nem, 1..x tipusok}
      Procent: Integer;       //  A bevetel szazalekos elosztasa, a tulajdonosnal marado resz
      RepeatLimit: Integer;   // Ha NULLA, ismetlodhet ugyanaz a zeneszam, ha nem, akkor nem
      AutoStepMin: Integer;   // Minimalisan ennyi masodperc utan lepked az albumok kozott
      AutoStepMax: Integer;   // Maximalisan ennyi masodperc utan lepked az albumok kozott
      BackStepTime: Integer;  // Ennyi masodperc utan lep vissza az album kepernyore
      PlayerCutOff :Integer;  // Ennyi masodperc utan lekeveri a zeneszamot. Nulla eseten nem.
      SysVolEnabled : Boolean;         {Kell-e rendszerhang}
//      MixMode: Byte;          // Mix mode - osszekeveri a szamokat 0-nem, 1-igen
      // DEMO
      NeedDemo: Boolean;      // Kell-e demoznia a zenegepnek
      DemoStartTime: Integer; // Ennyi masodperc utan kezdjen demozni - 0-folyamatosan demozik
      DemoFade: Integer;      // Demo lekeveres ennyi perc utan, 0 - vegigjatssza a szamot}
      DemoTop: Byte;            {Honnan legyen demo (top mennyi) 1-10, 2-20, ... 6-mind}

      FreeModeType: Byte;     // Free uzemmod tipusa: 0 - normal mode, 1- Short Free, 2- Long Free, 3- Remote Free

      NeedPinCode : Boolean;  // Kell-e megadni indulaskor pinkodot
      NetMinSpeed:Integer;    // Minimalisan szukseges internet sebesseg Mbit/sec
      End;


// Adatbazis kezeleshez
//Function WaitForConnectSQL(MyServerConnSQL: TServerConnSQL): Boolean;
//Function MyConnectSQL(MyServerConnSQL: TServerConnSQL): Boolean;

//Function ConnectSQL(MyServerConnSQL: TServerConnSQL): Boolean;
Procedure SQLDisConnect;

implementation

{Main create}
Constructor TDetMySQL.Create(Owner: TComponent);
Begin
  Inherited Create(Owner);
//  Display_Form.Top:=0;
//  Display_Form.Left:=0;
//  Display_Form.Width:= 1032;
//  Display_Form.Height:= 795;
//  Display_Form.Visible:= True;
//  Display_Form.DoubleBuffered := True;
//  Mainbg.Create(Owner);
//   Mainbg.Picture.LoadFromFile('main_bg4.jpg');
//  FFTPGetComplete := False;
End;


{ TODO -oRinger Attila -cCimke : SetMyConn }
Procedure TDetMySQL.SetDetConn(Host, User, Pass, Database: String);

// MyName : String;

begin
     DetConn.Hostname := Host;
     DetConn.User := User;
     DetConn.Password := Pass;
     DetConn.Database := Database;
//    MyName := MyConn1.User;
//    if MyName = 'RR' then    MyConn1.Connect;
//    MyConn1.SetUser(User);
end;


Function ConnectSQL(MyServerConnSQL: TServerConnSQL): Boolean;
begin
    DetMySql.SetDetConn(MyServerConnSQL.ServerName, MyServerConnSQL.Username,
            MyServerConnSQL.Password, MyServerConnSQL.Database);
    DetMySQL.DetConn.Connect;
    Result := DetMySQL.DetConn.Connected;
end;


// a zenegep lecsatlakozik a MySQL szerverrol
Procedure SQLDisConnect;
begin
    If DetMySQL.DetConn.Connected then DetMySql.DetConn.Disconnect;
end; // end of Procedure


Function MyConnectSQL(MyServerConnSQL: TServerConnSQL): Boolean;
Var I, J : Integer;
MyHiba : String;

begin
  Result := True; // Legyen kezdoerteke
  If DetMySql.DetConn.Connected then Exit; // ha van kapcsolat nem kell csatlakozni

      try ConnectSQL(MyServerConnSQL);

    Except
       on E : Exception do begin
          MyHiba := E.ClassName + ' - ' + E.Message;
//          SqlErrorLog(MyHiba);
          SQLDisConnect;  // szetkapcsolunk
       end;  // end of Exception
    End;  // end of Try

 If DetMySql.DetConn.Connected then Result := True else Result := False;

end;  // End of Function MyConnectSQL


end.


