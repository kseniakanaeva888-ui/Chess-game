object Form1: TForm1
  Left = 568
  Top = 82
  Caption = 'Chess'
  ClientHeight = 562
  ClientWidth = 651
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poDesigned
  TextHeight = 15
  object Image1: TImage
    Left = 8
    Top = 19
    Width = 480
    Height = 480
    OnMouseDown = Image1MouseDown
  end
  object Label3: TLabel
    Left = 494
    Top = 19
    Width = 72
    Height = 15
    Caption = #1056#1077#1078#1080#1084' '#1080#1075#1088#1099':'
  end
  object Panel1: TPanel
    Left = 0
    Top = 514
    Width = 651
    Height = 48
    Align = alBottom
    TabOrder = 0
    ExplicitTop = 497
    ExplicitWidth = 645
    object Label1: TLabel
      Left = 8
      Top = 16
      Width = 126
      Height = 15
      Caption = #1053#1072#1078#1084#1080#1090#1077' "'#1053#1086#1074#1072#1103' '#1080#1075#1088#1072'"'
    end
    object Label2: TLabel
      Left = 256
      Top = 16
      Width = 71
      Height = 15
      Caption = #1042#1088#1077#1084#1103' : 00:00'
    end
    object Button1: TButton
      Left = 481
      Top = 12
      Width = 75
      Height = 25
      Caption = #1053#1086#1074#1072#1103' '#1080#1075#1088#1072
      TabOrder = 0
      OnClick = Button1Click
    end
    object Button2: TButton
      Left = 568
      Top = 12
      Width = 75
      Height = 25
      Caption = #1042#1099#1093#1086#1076
      TabOrder = 1
      OnClick = Button2Click
    end
    object Button3: TButton
      Left = 344
      Top = 12
      Width = 123
      Height = 25
      Caption = #1055#1088#1077#1076#1083#1086#1078#1080#1090#1100' '#1085#1080#1095#1100#1102
      TabOrder = 2
      OnClick = Button3Click
    end
  end
  object ComboBox1: TComboBox
    Left = 494
    Top = 48
    Width = 149
    Height = 23
    TabOrder = 1
    Text = 'ComboBox1'
    OnChange = ComboBox1Change
    Items.Strings = (
      #1044#1074#1072' '#1080#1075#1088#1086#1082#1072
      #1051#1077#1075#1082#1080#1081' '
      #1057#1083#1086#1078#1085#1099#1081' ')
  end
  object Button4: TButton
    Left = 512
    Top = 474
    Width = 131
    Height = 25
    Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1080#1075#1088#1091
    TabOrder = 2
    OnClick = Button4Click
  end
  object Button5: TButton
    Left = 512
    Top = 432
    Width = 131
    Height = 25
    Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1089#1090#1072#1090#1080#1089#1090#1080#1082#1091
    TabOrder = 3
    OnClick = Button5Click
  end
  object Timer1: TTimer
    OnTimer = Timer1Timer
    Left = 504
    Top = 361
  end
  object SaveDialog1: TSaveDialog
    Left = 560
    Top = 368
  end
end
