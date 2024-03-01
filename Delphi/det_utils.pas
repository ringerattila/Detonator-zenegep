unit Det_utils;

interface

uses
  Windows, Messages, Classes, SysUtils, IdTCPConnection, IdTCPClient,
  IdFTP, IdHTTP;

function IniFile: string;
procedure ReadIniFile;
procedure DecodeServers(EncMyFTP, EncMySQL, EncMyMP3:String);

type
  TServerConnSQL = Record          //
    ServerName: String[40];          // Server
    UserName  : String[40];          // User
    Password  : String[40];          // Pass
    Database  : String[40];          // Database
    End;

  TServerConnFTP = Record          //
    ServerName: String[40];          // Server
    RemoteLib : String[128];      // tavoli konyvtar
    UserName  : String[40];          // User
    Password  : String[40];          // Pass
    End;


const
  A = 20;

var

   MyFTP: TIdFTP;

  AlapDir : String; // Az a konyvtar, ahol az aktualisan futo .exe program van.
  OldDir : String; // A régi program könyvtára, .ini-ből olvassuk be

    MyEncServerConnFTP : String;
    MyEncServerConnSQL : String;
    MyEncServerConnMP3 : String;
    MyServerConnMP3    : String;  // az MP3 file-ok elérése (http protokoll)

    NetDrive : String;   // INI file-ból ovassuk be
    NetMachine : Boolean; // a detonator.ini file-ban tároljuk az értéket
    TryURL : String; // ezt az URL-t nézi meg eldönteni, hogy van-e net
    PortBeolvasott: Integer;               // A Hardware-el melyik porton kommunikalunk
    MainDispX, MainDispY : Integer; {Főablak pozíciója}
      SerialToLog : String;  // YES = loggolni kell a soros forgalmat
      DetVer : String;     // detonator.exe verziószáma, parancssori paraméterként kapjuk
   LoaderVer : String;     // loader.exe verziószáma, parancssori paraméterként kapjuk

      MyServerConnFTP : TServerConnFTP;
    MyServerConnSQL : TServerConnSQL;


//  I : Integer;

implementation

uses Inifiles, shlobj; // start menu miatt
// {$R *.DFM}

// INI file kezelése
function IniFile: string;
begin
//  Result := ExtractFilePath(ParamStr(0)) + 'detonator.ini';
  Result := AlapDir + 'detonator.ini';
end;

// ReadIniFile
procedure ReadIniFile;
var
  vIni: TIniFile;
 // MyByte, MyBit, My01 : Integer;
//  myKey : String;

begin
  vIni := TIniFile.Create(IniFile);
  try

 // Beolvassuk a Szervereket
  MyEncServerConnSQL := vIni.ReadString('Servers', 'Server1S', '');
  MyEncServerConnFTP := vIni.ReadString('Servers', 'Server1F', '');
  MyEncServerConnMP3 := vIni.ReadString('Servers', 'Server1M', '');
//  Server2 := vIni.ReadString('Servers', 'Server2', '');


  NetDrive := vIni.ReadString('Settings', 'NetDrive', '');

  // Szeretnenk-e, hogy internetes gep legyen?
    if vIni.ReadString('Settings', 'NetMachine', '')='YES' then
      NetMachine := True else NetMachine := False;

  TryURL := vIni.ReadString('Settings', 'TryURL', '');

 // A zenegep COM-portja, ezen fogja keresni a hardvert.
  PortBeolvasott := StrToInt(vIni.ReadString('Settings', 'SerialPort', ''));


 // A foablak pozicioja
  MainDispX := StrToInt(vIni.ReadString('Settings', 'OrigPosX', ''));
  MainDispY := StrToInt(vIni.ReadString('Settings', 'OrigPosY', ''));

  // Beolvassuk a LOG-golas beallitasait
   SerialToLog := vIni.ReadString('Logs', 'Serial', '');

   DetVer := vIni.ReadString('Versions', 'Detonator', '');
   LoaderVer := vIni.ReadString('Versions', 'Loader', '');


 // A régi program könyvtára
  OldDir := vIni.ReadString('Loader', 'OldDir', '');


  finally
    vIni.Free;
  end;
end;

// 
function HexToInt(hex : String): Integer;
 const HexSymbols = '0123456789ABCDEF';
var i, J: integer;
//  Bla : String;

begin
    J:= 0; // A fordító hiányolja, ha nincs kezdőerték
  For i := 0 to 15 do begin
    if HexSymbols[i+1] = hex[1] then J := i * 16;
    end;

  For i := 0 to 15 do begin
    if HexSymbols[i+1] = hex[2] then J := J + i;
    end;
  Result := J;

end;


// Belépési jelszavak dekódolása
procedure DecodeServers(EncMyFTP, EncMySQL, EncMyMP3:String);
var
 // MyByte, MyBit, My01 : Integer;
//  myKey : String;
EncLength, I, K : Integer;
NewChar : byte;
HexChar : String;
NyiltKiSzoveg : String;
 MyDatas : TStringList;

 begin
// SQL elérés dekódolása
  NyiltKiSzoveg := '';
  EncLength := Length(EncMySQL);
  I := 1;
  K := 1;
  While I < EncLength do begin
  HexChar := EncMySQL[I] + EncMySQL[I+1];  // hexa számonként beolvassuk
  NewChar := HexToInt(HexChar);
//  NewChar := Ord(KodoltBeSzoveg.Text[I]); // Minden egyes karaktert külön veszünk
  NewChar := NewChar XOR (55+K);   // Mindig növekvő kulccsal XOR-oljuk
   NyiltKiSzoveg := NyiltKiSzoveg + Chr(NewChar);
   I := I + 2;
   K := K +1;
   end;
 //  P := Pos('-', NyiltKiSzoveg);
//   Server2.Text := '';

  MyDatas := TStringList.Create;
  ExtractStrings( ['-','-','-'],[' '], PChar(NyiltKiSzoveg), MyDatas);
   MyServerConnSQL.ServerName := MyDatas[0];
   MyServerConnSQL.Database := MyDatas[1];
   MyServerConnSQL.Username := MyDatas[2];
   MyServerConnSQL.Password := MyDatas[3];

  MyDatas.Free;
// FTP elérés dekódolása
   NyiltKiSzoveg := '';
  EncLength := Length(EncMyFTP);
  I := 1;
  K := 1;
  While I < EncLength do begin
  HexChar := EncMyFTP[I] + EncMyFTP[I+1];  // hexa számonként beolvassuk
  NewChar := HexToInt(HexChar);
//  NewChar := Ord(KodoltBeSzoveg.Text[I]); // Minden egyes karaktert külön veszünk
  NewChar := NewChar XOR (55+K);   // Mindig növekvő kulccsal XOR-oljuk
   NyiltKiSzoveg := NyiltKiSzoveg + Chr(NewChar);
   I := I + 2;
   K := K +1;
   end;
 //  P := Pos('-', NyiltKiSzoveg);
//   Server2.Text := '';

  MyDatas := TStringList.Create;
  ExtractStrings( ['-','-','-'],[' '], PChar(NyiltKiSzoveg), MyDatas);
   MyServerConnFTP.ServerName := MyDatas[0];
   MyServerConnFTP.RemoteLib := MyDatas[1];
   MyServerConnFTP.Username := MyDatas[2];
   MyServerConnFTP.Password := MyDatas[3];

  MyDatas.Free;


// MP3 elérés dekódolása
   NyiltKiSzoveg := '';
  EncLength := Length(EncMyMP3);
  I := 1;
  K := 1;
  While I < EncLength do begin
  HexChar := EncMyMP3[I] + EncMyMP3[I+1];  // hexa számonként beolvassuk
  NewChar := HexToInt(HexChar);
//  NewChar := Ord(KodoltBeSzoveg.Text[I]); // Minden egyes karaktert külön veszünk
  NewChar := NewChar XOR (55+K);   // Mindig növekvő kulccsal XOR-oljuk
   NyiltKiSzoveg := NyiltKiSzoveg + Chr(NewChar);
   I := I + 2;
   K := K +1;
   end;
 //  P := Pos('-', NyiltKiSzoveg);
//   Server2.Text := '';

  MyDatas := TStringList.Create;
  ExtractStrings( ['-','-','-'],[' '], PChar(NyiltKiSzoveg), MyDatas);
   MyServerConnMP3 := MyDatas[0];
  MyDatas.Free;

  end;


end.

