object Form1: TForm1
  Left = 0
  Top = 0
  Width = 691
  Height = 622
  HorzScrollBar.Position = 941
  AutoScroll = True
  Caption = 'Analizador'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnKeyDown = FormKeyDown
  OnMouseDown = FormMouseDown
  OnMouseWheel = FormMouseWheel
  PixelsPerInch = 96
  TextHeight = 13
  object Image1: TImage
    Left = -941
    Top = 0
    Width = 1600
    Height = 960
    Enabled = False
  end
  object Panel1: TPanel
    Left = 104
    Top = 88
    Width = 105
    Height = 441
    BorderStyle = bsSingle
    TabOrder = 0
    Visible = False
    OnMouseDown = Panel1MouseDown
  end
  object Panel2: TPanel
    Left = 120
    Top = 88
    Width = 225
    Height = 441
    BorderStyle = bsSingle
    Caption = 'Panel1'
    TabOrder = 1
    Visible = False
    object RichEdit1: TRichEdit
      Left = 1
      Top = 29
      Width = 219
      Height = 407
      Align = alClient
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      ScrollBars = ssBoth
      TabOrder = 0
      Zoom = 100
    end
    object StatusBar1: TStatusBar
      Left = 1
      Top = 1
      Width = 219
      Height = 28
      Align = alTop
      Panels = <
        item
          Text = '  Seg  3600  Tick  59  Ops  99999'
          Width = 158
        end
        item
          Alignment = taCenter
          Text = '>'
          Width = 30
        end
        item
          Alignment = taRightJustify
          Text = 'R   '
          Width = 30
        end>
      OnMouseDown = StatusBar1MouseDown
      OnMouseMove = StatusBar1MouseMove
      OnMouseUp = StatusBar1MouseUp
    end
  end
  object MainMenu1: TMainMenu
    Left = 16
    Top = 16
    object Fichero1: TMenuItem
      Caption = 'Fichero'
      object Abrir1: TMenuItem
        Caption = 'Abrir'
        OnClick = Abrir1Click
      end
      object Cerrar1: TMenuItem
        Caption = 'Cerrar'
        Enabled = False
        OnClick = Cerrar1Click
      end
      object Salir1: TMenuItem
        Caption = 'Salir'
        OnClick = Salir1Click
      end
    end
    object Seguimiento1: TMenuItem
      Caption = 'Seguimiento'
      Enabled = False
      object Inicio1: TMenuItem
        Caption = 'Inicio'
        ShortCut = 16457
        OnClick = Inicio1Click
      end
      object SigOp1: TMenuItem
        Caption = 'Sig Op'
        ShortCut = 16463
        OnClick = SigOp1Click
      end
      object SigTick1: TMenuItem
        Caption = 'Sig Tick'
        ShortCut = 16468
        OnClick = SigTick1Click
      end
      object SigSeg1: TMenuItem
        Caption = 'Sig Seg'
        ShortCut = 16455
        OnClick = SigSeg1Click
      end
      object AntSeg1: TMenuItem
        Caption = 'Ant Seg'
        ShortCut = 16449
        OnClick = AntSeg1Click
      end
      object Final1: TMenuItem
        Caption = 'Final'
        ShortCut = 16454
        OnClick = Final1Click
      end
      object StartStop1: TMenuItem
        Caption = 'Start/Stop'
        ShortCut = 16467
        OnClick = StartStop1Click
      end
      object Parado1: TMenuItem
        Caption = 'Parado'
        Checked = True
        ShortCut = 16432
        OnClick = Parado1Click
      end
      object Frecuencia15Hz1: TMenuItem
        Caption = '  15 Ticks/s'
        ShortCut = 16433
        OnClick = Frecuencia15Hz1Click
      end
      object Frecuencia30Hz1: TMenuItem
        Caption = '  30 Ticks/s'
        ShortCut = 16434
        OnClick = Frecuencia30Hz1Click
      end
      object Frecuencia45Hz1: TMenuItem
        Caption = '  45 Ticks/s'
        ShortCut = 16435
        OnClick = Frecuencia45Hz1Click
      end
      object Frecuencia60Hz1: TMenuItem
        Caption = '  60 Ticks/s'
        ShortCut = 16436
        OnClick = Frecuencia60Hz1Click
      end
      object Frecuencia90Hz1: TMenuItem
        Caption = '  90 Ticks/s'
        ShortCut = 16437
        OnClick = Frecuencia90Hz1Click
      end
      object Frecuencia120Hz1: TMenuItem
        Caption = '120 Ticks/s'
        ShortCut = 16438
        OnClick = Frecuencia120Hz1Click
      end
      object Frecuencia240Hz1: TMenuItem
        Caption = '240 Ticks/s'
        ShortCut = 16439
        OnClick = Frecuencia240Hz1Click
      end
      object Frecuencia480Hz1: TMenuItem
        Caption = '480 Ticks/s'
        ShortCut = 16440
        OnClick = Frecuencia480Hz1Click
      end
      object Frecuencia960Hz1: TMenuItem
        Caption = '960 Ticks/s'
        ShortCut = 16441
        OnClick = Frecuencia960Hz1Click
      end
      object Continuo1: TMenuItem
        Caption = 'Continuo'
        ShortCut = 16451
        OnClick = Continuo1Click
      end
    end
    object Ver1: TMenuItem
      Caption = 'Ver'
      Enabled = False
      object Ir_a1: TMenuItem
        Caption = 'Ir a'
        OnClick = Ir_a1Click
      end
      object ScrollSeg1: TMenuItem
        Caption = 'Scroll Seg'
        ShortCut = 16470
        OnClick = ScrollSeg1Click
      end
      object nOpsTick1: TMenuItem
        Caption = 'nOpsTick'
        OnClick = nOpsTick1Click
      end
    end
    object Ayuda1: TMenuItem
      Caption = 'Ayuda'
      object Ayuda2: TMenuItem
        Caption = 'C'#243'digo de Minix'
        ShortCut = 121
        OnClick = Ayuda2Click
      end
      object usrsrcmpx386s1: TMenuItem
        Caption = 'kernel/mpx386.s'
        OnClick = usrsrcmpx386s1Click
      end
      object usrsrcmpx386s2: TMenuItem
        Caption = 'kernel/i8259.c'
        OnClick = usrsrcmpx386s2Click
      end
      object usrsrckernelexceptionc1: TMenuItem
        Caption = 'kernel/exception.c'
        OnClick = usrsrckernelexceptionc1Click
      end
      object usrsrcdriversttykeyboardc1: TMenuItem
        Caption = 'drivers/tty/keyboard.c'
        OnClick = usrsrcdriversttykeyboardc1Click
      end
      object usrsrcdriversttykeyboardc2: TMenuItem
        Caption = 'drivers/at_wini/at_wini.c'
        OnClick = usrsrcdriversttykeyboardc2Click
      end
      object Sobre1: TMenuItem
        Caption = 'Sobre...'
        ShortCut = 123
        OnClick = Sobre1Click
      end
    end
  end
  object OpenDialog1: TOpenDialog
    DefaultExt = 'bin'
    FileName = 'Y:\log_e9.bin'
    Filter = 'bin|*.bin'
    InitialDir = 'Y:\'
    Left = 104
    Top = 16
  end
  object Timer1: TTimer
    Enabled = False
    OnTimer = Timer1Timer
    Left = 192
    Top = 16
  end
  object TimerIndice: TTimer
    Enabled = False
    OnTimer = TimerIndiceTimer
    Left = 248
    Top = 16
  end
end
