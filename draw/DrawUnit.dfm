object DrawOpenGL: TDrawOpenGL
  Left = 0
  Top = 0
  Caption = 'DrawOpenGL'
  ClientHeight = 640
  ClientWidth = 1180
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  KeyPreview = True
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnMouseDown = FormMouseDown
  OnMouseMove = FormMouseMove
  OnMouseUp = FormMouseUp
  OnMouseWheel = FormMouseWheel
  OnPaint = FormPaint
  OnResize = FormResize
  TextHeight = 15
  object PnlFerramentas: TPanel
    Left = 0
    Top = 0
    Width = 1180
    Height = 98
    Align = alTop
    BevelOuter = bvNone
    ParentBackground = False
    TabOrder = 0
    object LblProfundidade: TLabel
      Left = 448
      Top = 4
      Width = 37
      Height = 15
      Caption = 'Zo fixo'
    end
    object LblStatus: TLabel
      Left = 1068
      Top = 8
      Width = 108
      Height = 84
      AutoSize = False
      Caption = 'Pronto'
      WordWrap = True
    end
    object LblFiguras: TLabel
      Left = 6
      Top = 74
      Width = 42
      Height = 15
      Caption = 'Figuras:'
    end
    object RGFigura: TRadioGroup
      Left = 4
      Top = 2
      Width = 260
      Height = 62
      Caption = 'Figura'
      Columns = 3
      ItemIndex = 0
      Items.Strings = (
        'Linha'
        'Quadril'#225'tero'
        'Elipse')
      TabOrder = 0
      OnClick = RGFiguraClick
    end
    object RGVisao: TRadioGroup
      Left = 268
      Top = 2
      Width = 174
      Height = 62
      Caption = 'Vis'#227'o'
      Columns = 3
      ItemIndex = 0
      Items.Strings = (
        'XoY'
        'XoZ'
        'YoZ')
      TabOrder = 1
      OnClick = RGVisaoClick
    end
    object EdtProfundidade: TEdit
      Left = 448
      Top = 22
      Width = 64
      Height = 23
      TabOrder = 2
      Text = '0'
    end
    object CBCor: TColorBox
      Left = 518
      Top = 22
      Width = 120
      Height = 22
      Selected = clRed
      TabOrder = 3
    end
    object BtnGiraXo: TButton
      Left = 644
      Top = 6
      Width = 70
      Height = 26
      Caption = 'Gira Xo'
      TabOrder = 4
      OnClick = BtnGiraXoClick
    end
    object BtnGiraYo: TButton
      Left = 718
      Top = 6
      Width = 70
      Height = 26
      Caption = 'Gira Yo'
      TabOrder = 5
      OnClick = BtnGiraYoClick
    end
    object BtnGiraZo: TButton
      Left = 792
      Top = 6
      Width = 70
      Height = 26
      Caption = 'Gira Zo'
      TabOrder = 6
      OnClick = BtnGiraZoClick
    end
    object ChkPerspectiva: TCheckBox
      Left = 648
      Top = 40
      Width = 90
      Height = 19
      Caption = 'Visualizar 3D'
      TabOrder = 7
      OnClick = ChkPerspectivaClick
    end
    object BtnLimpar: TButton
      Left = 868
      Top = 6
      Width = 60
      Height = 26
      Caption = 'Limpar'
      TabOrder = 8
      OnClick = BtnLimparClick
    end
    object BtnSalvarDEN: TButton
      Left = 932
      Top = 6
      Width = 60
      Height = 26
      Hint = 'Salvar .DEN'
      Caption = 'Salvar'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 9
      OnClick = BtnSalvarDENClick
    end
    object BtnAbrirGLB: TButton
      Left = 868
      Top = 36
      Width = 60
      Height = 26
      Caption = 'Abrir .glb'
      TabOrder = 10
      OnClick = BtnAbrirGLBClick
    end
    object BtnExportarGLB: TButton
      Left = 932
      Top = 36
      Width = 60
      Height = 26
      Hint = 'Exportar .glb'
      Caption = 'Exp. .glb'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 11
      OnClick = BtnExportarGLBClick
    end
    object BtnAbrirDEN: TButton
      Left = 996
      Top = 6
      Width = 68
      Height = 26
      Caption = 'Abrir .DEN'
      TabOrder = 12
      OnClick = BtnAbrirDENClick
    end
    object BtnDupTeta: TButton
      Left = 740
      Top = 36
      Width = 62
      Height = 26
      Caption = 'Dup. Teta'
      TabOrder = 13
      OnClick = BtnDupTetaClick
    end
    object BtnDupAlpha: TButton
      Left = 804
      Top = 36
      Width = 62
      Height = 26
      Caption = 'Dup. Alpha'
      TabOrder = 14
      OnClick = BtnDupAlphaClick
    end
    object CBFiguras: TComboBox
      Left = 54
      Top = 70
      Width = 480
      Height = 23
      Style = csDropDownList
      DropDownCount = 20
      TabOrder = 15
      OnChange = CBFigurasChange
    end
    object ChkEditarVertices: TCheckBox
      Left = 448
      Top = 48
      Width = 100
      Height = 19
      Hint = 'Arraste os v'#233'rtices das figuras selecionadas para remodelar'
      Caption = 'Editar v'#233'rtices'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 16
      OnClick = ChkEditarVerticesClick
    end
    object ChkEncaixe: TCheckBox
      Left = 552
      Top = 48
      Width = 88
      Height = 19
      Hint = 'O mouse gruda nos v'#233'rtices das figuras'
      Caption = 'Encaixar'
      Checked = True
      ParentShowHint = False
      ShowHint = True
      State = cbChecked
      TabOrder = 17
    end
    object BtnDesfazer: TButton
      Left = 748
      Top = 69
      Width = 60
      Height = 25
      Hint = 'Desfazer (Ctrl+Z)'
      Caption = 'Desfazer'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 18
      OnClick = BtnDesfazerClick
    end
    object BtnRefazer: TButton
      Left = 810
      Top = 69
      Width = 60
      Height = 25
      Hint = 'Refazer (Ctrl+Y)'
      Caption = 'Refazer'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 19
      OnClick = BtnRefazerClick
    end
    object LblExtrusao: TLabel
      Left = 876
      Top = 74
      Width = 32
      Height = 15
      Caption = 'Altura'
    end
    object EdtExtrusao: TEdit
      Left = 912
      Top = 70
      Width = 50
      Height = 23
      TabOrder = 20
      Text = '50'
    end
    object BtnExtrudar: TButton
      Left = 966
      Top = 69
      Width = 96
      Height = 25
      Hint = 'D'#225' altura '#224's figuras selecionadas (sem sele'#231#227'o, '#224' escolhida na lista)'
      Caption = 'Extrudar'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 21
      OnClick = BtnExtrudarClick
    end
    object ChkSelecionar: TCheckBox
      Left = 540
      Top = 72
      Width = 112
      Height = 19
      Hint = 'Clique nas figuras ou arraste um ret'#226'ngulo em volta delas (Esc limpa)'
      Caption = 'Selecionar figuras'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 22
      OnClick = ChkSelecionarClick
    end
    object BtnLimparSelecao: TButton
      Left = 656
      Top = 69
      Width = 88
      Height = 25
      Caption = 'Limpar sele'#231#227'o'
      TabOrder = 23
      OnClick = BtnLimparSelecaoClick
    end
  end
  object PnlEdicao: TPanel
    Left = 880
    Top = 98
    Width = 300
    Height = 542
    Align = alRight
    BevelOuter = bvNone
    ParentBackground = False
    TabOrder = 1
    object LblEdicao: TLabel
      Left = 6
      Top = 4
      Width = 288
      Height = 15
      AutoSize = False
      Caption = 'Escolha uma figura na lista'
    end
    object VLEdicao: TValueListEditor
      Left = 4
      Top = 24
      Width = 292
      Height = 478
      Anchors = [akLeft, akTop, akRight, akBottom]
      TabOrder = 0
      TitleCaptions.Strings = (
        'Campo'
        'Valor')
      ColWidths = (
        150
        136)
    end
    object BtnAplicar: TButton
      Left = 4
      Top = 510
      Width = 90
      Height = 26
      Anchors = [akLeft, akBottom]
      Caption = 'Aplicar'
      TabOrder = 1
      OnClick = BtnAplicarClick
    end
    object BtnRecarregar: TButton
      Left = 98
      Top = 510
      Width = 90
      Height = 26
      Anchors = [akLeft, akBottom]
      Caption = 'Recarregar'
      TabOrder = 2
      OnClick = BtnRecarregarClick
    end
  end
  object SaveDialog1: TSaveDialog
    Left = 40
    Top = 100
  end
  object OpenDialog1: TOpenDialog
    Left = 120
    Top = 100
  end
end
