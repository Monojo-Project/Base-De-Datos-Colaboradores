#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QVector>
#include <QResizeEvent>
#include <QKeyEvent>
#include <QSettings>
#include <QCoreApplication>
#include <algorithm>

// =====================================================================
// ESTRUCTURA DE DATOS
// =====================================================================
struct Contributor {
    QString name;
    QString slogan;
    QString fecha;
    QString rango;
    QString descripcion;
    QString correo;
    QString numero;
    QString proyecto;
    QString trabajo;
    QString razon;
};

// =====================================================================
// COMPONENTE: Carta Adaptable Estilo Pokémon TCG
// =====================================================================
class ContributorCard : public QWidget {
public:
    ContributorCard(const Contributor &c, bool isDark, QWidget *parent = nullptr) : QWidget(parent) {
        setFixedSize(270, 420);
        setAttribute(Qt::WA_TranslucentBackground, true);

        QString rangoLower = c.rango.toLower().trimmed();
        QString neonColor = isDark ? "#9B51E0" : "#722ED1";

        if (rangoLower.contains("fundador")) {
            neonColor = isDark ? "#00FF41" : "#00AA2C";
        } else if (rangoLower.startsWith("aprendiz")) {
            neonColor = isDark ? "#FFD700" : "#C49000";
        } else if (rangoLower.contains("alto nivel") || rangoLower.contains("experimentado") || rangoLower.contains("programador")) {
            neonColor = isDark ? "#00E5FF" : "#007B99";
        }

        QString bgCard = isDark ? "#141414" : "#FFFFFF";
        QString txtMain = isDark ? "#FFFFFF" : "#121212";
        QString txtSlogan = isDark ? "#888888" : "#666666";
        QString bgArt = isDark ? "#222222" : "#E8E8E8";
        QString bgDesc = isDark ? "#1A1A1A" : "#F5F5F5";
        QString txtDesc = isDark ? "#DCDCDC" : "#333333";

        QFrame *innerCard = new QFrame(this);
        innerCard->setObjectName("InnerCard");

        QVBoxLayout *outerLayout = new QVBoxLayout(this);
        outerLayout->setContentsMargins(0, 0, 0, 0);
        outerLayout->addWidget(innerCard);

        innerCard->setStyleSheet(QString(
            "#InnerCard { background-color: %1; border: 2px solid %2; border-radius: 8px; }"
            "QLabel { border: none; font-family: 'Consolas'; color: %3; background: transparent; }"
        ).arg(bgCard, neonColor, txtMain));

        QVBoxLayout *layout = new QVBoxLayout(innerCard);
        layout->setContentsMargins(12, 12, 12, 12);
        layout->setSpacing(6);

        QLabel *lblName = new QLabel(c.name.toUpper(), innerCard);
        lblName->setStyleSheet("font-size: 13px; font-weight: bold;");
        lblName->setWordWrap(true);
        layout->addWidget(lblName);

        QLabel *lblSlogan = new QLabel(c.slogan, innerCard);
        lblSlogan->setStyleSheet(QString("font-size: 10px; font-style: italic; color: %1;").arg(txtSlogan));
        lblSlogan->setWordWrap(true);
        layout->addWidget(lblSlogan);

        QFrame *artFrame = new QFrame(innerCard);
        artFrame->setStyleSheet(QString("background-color: %1; border: 1px solid %2; border-radius: 4px;")
                                .arg(bgArt, isDark ? "#333333" : "#D0D0D0"));
        artFrame->setFixedHeight(45);
        QVBoxLayout *artLayout = new QVBoxLayout(artFrame);
        artLayout->setContentsMargins(4, 4, 4, 4);
        QLabel *lblProj = new QLabel("⚙️ " + (c.proyecto.isEmpty() ? "LYNDS PROJECT" : c.proyecto), artFrame);
        lblProj->setStyleSheet(QString("font-size: 11px; font-weight: bold; color: %1; background: transparent;").arg(txtMain));
        lblProj->setAlignment(Qt::AlignCenter);
        artLayout->addWidget(lblProj);
        layout->addWidget(artFrame);

        auto addStatRow = [&](const QString &label, QString val, const QString &valColor) {
            if (val.isEmpty() || val.contains("PONER")) val = "---";
            QHBoxLayout *row = new QHBoxLayout();
            QLabel *lblKey = new QLabel(label, innerCard);
            lblKey->setStyleSheet(QString("font-size: 10px; font-weight: bold; color: %1;").arg(isDark ? "#666666" : "#888888"));
            lblKey->setFixedWidth(65);
            QLabel *lblVal = new QLabel(val, innerCard);
            lblVal->setStyleSheet(QString("font-size: 10px; color: %1;").arg(valColor));
            lblVal->setWordWrap(true);
            row->addWidget(lblKey);
            row->addWidget(lblVal, 1);
            layout->addLayout(row);
        };

        QString numDisp = c.numero.trimmed();
        if (numDisp.contains("PONER") || numDisp.contains("NO TIENE") || numDisp.isEmpty()) {
            numDisp = "??";
        }

        addStatRow("Rango:", c.rango, neonColor);
        addStatRow("Trabajo:", c.trabajo, txtMain);
        addStatRow("Fecha:", c.fecha, isDark ? "#A0A0A0" : "#555555");
        addStatRow("Correo:", c.correo, isDark ? "#A0A0A0" : "#555555");
        addStatRow("Número:", numDisp, neonColor);

        QFrame *descFrame = new QFrame(innerCard);
        descFrame->setStyleSheet(QString("background-color: %1; border-radius: 4px;").arg(bgDesc));
        QVBoxLayout *descLayout = new QVBoxLayout(descFrame);
        descLayout->setContentsMargins(6, 6, 6, 6);
        QLabel *lblDesc = new QLabel(c.descripcion.isEmpty() ? "Sin descripción registrada." : c.descripcion, descFrame);
        lblDesc->setStyleSheet(QString("font-size: 10px; color: %1; background: transparent;").arg(txtDesc));
        lblDesc->setWordWrap(true);
        lblDesc->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        descLayout->addWidget(lblDesc);
        layout->addWidget(descFrame, 1);

        if (!c.razon.isEmpty() && !c.razon.contains("PONER")) {
            QFrame *line = new QFrame(innerCard);
            line->setFixedHeight(1);
            line->setStyleSheet(QString("background-color: %1; border: none;").arg(isDark ? "#252525" : "#E0E0E0"));
            layout->addWidget(line);

            QLabel *lblRazon = new QLabel(QString("\"%1\"").arg(c.razon), innerCard);
            lblRazon->setStyleSheet(QString("font-size: 10px; font-style: italic; color: %1;").arg(neonColor));
            lblRazon->setWordWrap(true);
            lblRazon->setAlignment(Qt::AlignCenter);
            layout->addWidget(lblRazon);
        }
    }
};

// =====================================================================
// COMPONENTE: Contenedor Responsivo Grid
// =====================================================================
class CardsGridWidget : public QWidget {
public:
    CardsGridWidget(QWidget *parent = nullptr) : QWidget(parent) {
        gridLayout = new QGridLayout(this);
        gridLayout->setContentsMargins(15, 15, 15, 15);
        gridLayout->setSpacing(18);
        gridLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    }

    void setCards(const QVector<ContributorCard*> &newCards) {
        while (QLayoutItem *item = gridLayout->takeAt(0)) {
            if (item->widget()) item->widget()->hide();
            delete item;
        }
        cards = newCards;
        rearrangeGrid();
    }

protected:
    void resizeEvent(QResizeEvent *event) override {
        QWidget::resizeEvent(event);
        rearrangeGrid();
    }

private:
    QGridLayout *gridLayout;
    QVector<ContributorCard*> cards;

    void rearrangeGrid() {
        if (cards.isEmpty()) return;
        int viewWidth = width();
        int cardWidth = 270 + 18;
        int columns = qMax(1, viewWidth / cardWidth);

        for (int i = 0; i < cards.size(); ++i) {
            int row = i / columns;
            int col = i % columns;
            gridLayout->addWidget(cards[i], row, col);
            cards[i]->show();
        }
    }
};

// =====================================================================
// INTERFAZ PRINCIPAL DE LA APLICACIÓN
// =====================================================================
class LyndsCorpApp : public QMainWindow {
public:
    LyndsCorpApp() {
        setWindowTitle("Lynds Corp. - Base de Datos de Colaboradores");
        resize(980, 750);

        configPath = QDir(QCoreApplication::applicationDirPath()).filePath("lynds-cards.conf");
        QSettings settings(configPath, QSettings::IniFormat);
        isDarkMode = settings.value("Theme/Dark", true).toBool();

        QWidget *centralWidget = new QWidget(this);
        mainLayout = new QVBoxLayout(centralWidget);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);

        filterPanel = new QFrame(this);
        filterPanel->setFixedHeight(45);

        QHBoxLayout *filterLayout = new QHBoxLayout(filterPanel);
        filterLayout->setContentsMargins(15, 0, 15, 0);
        filterLayout->setSpacing(10);
        filterLayout->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

        lblSearch = new QLabel("BUSCAR:", filterPanel);
        txtSearch = new QLineEdit(filterPanel);
        txtSearch->setFixedWidth(150);
        txtSearch->setFixedHeight(26);

        lblRange = new QLabel("RANGO:", filterPanel);
        cmbRange = new QComboBox(filterPanel);
        cmbRange->setFixedWidth(130);
        cmbRange->setFixedHeight(26);

        lblSort = new QLabel("ORDENAR:", filterPanel);
        cmbSort = new QComboBox(filterPanel);
        cmbSort->addItems({"Número", "Nombre", "Proyecto"});
        cmbSort->setFixedWidth(100);
        cmbSort->setFixedHeight(26);

        btnTheme = new QPushButton(filterPanel);
        btnTheme->setMinimumWidth(130);
        btnTheme->setFixedHeight(26);

        filterLayout->addWidget(lblSearch);
        filterLayout->addWidget(txtSearch);
        filterLayout->addSpacing(10);
        filterLayout->addWidget(lblRange);
        filterLayout->addWidget(cmbRange);
        filterLayout->addSpacing(10);
        filterLayout->addWidget(lblSort);
        filterLayout->addWidget(cmbSort);
        filterLayout->addStretch();
        filterLayout->addWidget(btnTheme);
        mainLayout->addWidget(filterPanel);

        scrollArea = new QScrollArea(this);
        scrollArea->setWidgetResizable(true);
        gridWidget = new CardsGridWidget(scrollArea);
        scrollArea->setWidget(gridWidget);
        mainLayout->addWidget(scrollArea);

        setCentralWidget(centralWidget);

        applyGlobalThemeStyle();
        loadContributors();

        connect(txtSearch, &QLineEdit::textChanged, this, &LyndsCorpApp::applyFilters);
        connect(cmbRange, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &LyndsCorpApp::applyFilters);
        connect(cmbSort, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &LyndsCorpApp::applyFilters);
        connect(btnTheme, &QPushButton::clicked, this, &LyndsCorpApp::toggleTheme);
    }

protected:
    // Captura el evento de presionar teclas en la ventana principal
    void keyPressEvent(QKeyEvent *event) override {
        if (event->key() == Qt::Key_F11) {
            if (isFullScreen()) {
                showNormal(); // Restaura el tamaño original de la ventana
            } else {
                showFullScreen(); // Pone la aplicación en pantalla completa
            }
            event->accept(); // Indica que el evento ya fue manejado
        } else {
            QMainWindow::keyPressEvent(event); // Pasa el resto de teclas al comportamiento base
        }
    }

private:
    QVBoxLayout *mainLayout;
    QFrame *filterPanel;
    QLabel *lblSearch;
    QLineEdit *txtSearch;
    QLabel *lblRange;
    QComboBox *cmbRange;
    QLabel *lblSort;
    QComboBox *cmbSort;
    QPushButton *btnTheme;
    QScrollArea *scrollArea;
    CardsGridWidget *gridWidget;

    bool isDarkMode;
    QString configPath;
    QVector<Contributor> allContributors;
    QVector<ContributorCard*> activeCards;

    void applyGlobalThemeStyle() {
        if (isDarkMode) {
            setStyleSheet("QMainWindow { background-color: #0D0D0D; }");
            filterPanel->setStyleSheet("QFrame { background-color: #111111; border-bottom: 1px solid #222222; }");
            scrollArea->setStyleSheet("QScrollArea { background-color: #0D0D0D; border: none; } "
                                      "QScrollBar:vertical { background-color: #0D0D0D; width: 10px; } "
                                      "QScrollBar::handle:vertical { background-color: #252525; border-radius: 5px; } "
                                      "QScrollBar::handle:vertical:hover { background-color: #00FF41; }");

            QString labelStyle = "font-family: 'Consolas'; font-size: 11px; font-weight: bold; color: #00FF41; border: none; background: transparent;";
            lblSearch->setStyleSheet(labelStyle); lblRange->setStyleSheet(labelStyle); lblSort->setStyleSheet(labelStyle);

            txtSearch->setStyleSheet("QLineEdit { background-color: #1A1A1A; color: #FFFFFF; font-family: 'Consolas'; border: 1px solid #333333; padding: 0px 6px; border-radius: 3px; } QLineEdit:focus { border: 1px solid #00FF41; }");

            QString comboStyle = "QComboBox { background-color: #1A1A1A; color: #FFFFFF; font-family: 'Consolas'; border: 1px solid #333333; padding: 0px 6px; border-radius: 3px; padding-right: 15px; }"
                                 "QComboBox::drop-down { border: none; subcontrol-origin: padding; subcontrol-position: top right; width: 15px; }"
                                 "QComboBox QAbstractItemView { background-color: #1A1A1A; color: #FFFFFF; border: 1px solid #333333; selection-background-color: #00FF41; selection-color: #0D0D0D; }";
            cmbRange->setStyleSheet(comboStyle); cmbSort->setStyleSheet(comboStyle);

            btnTheme->setText("☀️ MODO CLARO");
            btnTheme->setStyleSheet("QPushButton { background-color: #1C1C1C; color: #FFFFFF; font-family: 'Consolas'; font-weight: bold; font-size: 10px; border: 1px solid #444444; padding: 0px; border-radius: 3px; }"
                                    "QPushButton:hover { background-color: #252525; border: 1px solid #00FF41; color: #00FF41; }");
        } else {
            setStyleSheet("QMainWindow { background-color: #F0F2F5; }");
            filterPanel->setStyleSheet("QFrame { background-color: #E4E6EB; border-bottom: 1px solid #D0D2D6; }");
            scrollArea->setStyleSheet("QScrollArea { background-color: #F0F2F5; border: none; } "
                                      "QScrollBar:vertical { background-color: #F0F2F5; width: 10px; } "
                                      "QScrollBar::handle:vertical { background-color: #C0C2C6; border-radius: 5px; } "
                                      "QScrollBar::handle:vertical:hover { background-color: #888888; }");

            QString labelStyle = "font-family: 'Consolas'; font-size: 11px; font-weight: bold; color: #007B99; border: none; background: transparent;";
            lblSearch->setStyleSheet(labelStyle); lblRange->setStyleSheet(labelStyle); lblSort->setStyleSheet(labelStyle);

            txtSearch->setStyleSheet("QLineEdit { background-color: #FFFFFF; color: #121212; font-family: 'Consolas'; border: 1px solid #B0B3B8; padding: 0px 6px; border-radius: 3px; } QLineEdit:focus { border: 1px solid #007B99; }");

            QString comboStyle = "QComboBox { background-color: #FFFFFF; color: #121212; font-family: 'Consolas'; border: 1px solid #B0B3B8; padding: 0px 6px; border-radius: 3px; padding-right: 15px; }"
                                 "QComboBox::drop-down { border: none; subcontrol-origin: padding; subcontrol-position: top right; width: 15px; }"
                                 "QComboBox QAbstractItemView { background-color: #FFFFFF; color: #121212; border: 1px solid #B0B3B8; selection-background-color: #007B99; selection-color: #FFFFFF; }";
            cmbRange->setStyleSheet(comboStyle); cmbSort->setStyleSheet(comboStyle);

            btnTheme->setText("MODO OSCURO");
            btnTheme->setStyleSheet("QPushButton { background-color: #FFFFFF; color: #121212; font-family: 'Consolas'; font-weight: bold; font-size: 10px; border: 1px solid #B0B3B8; padding: 0px; border-radius: 3px; }"
                                    "QPushButton:hover { background-color: #F0F2F5; border: 1px solid #007B99; color: #007B99; }");
        }
    }

    void toggleTheme() {
        isDarkMode = !isDarkMode;

        QSettings settings(configPath, QSettings::IniFormat);
        settings.setValue("Theme/Dark", isDarkMode);
        settings.sync();

        applyGlobalThemeStyle();
        applyFilters();
    }

    void loadContributors() {
        QString rawPath = "~/COLABORADORES.md";
        QString resolvedPath = rawPath;
        if (resolvedPath.startsWith("~")) {
            resolvedPath.replace(0, 1, QDir::homePath());
        }

        QFile file(resolvedPath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QLabel *errLabel = new QLabel("SISTEMA DE ARCHIVOS LYNDS: Error de enlace.\n\nFalta el fichero en " + rawPath, gridWidget);
            errLabel->setStyleSheet("font-family: 'Consolas'; font-size: 13px; color: #FF007F; font-weight: bold;");
            errLabel->setAlignment(Qt::AlignCenter);
            QVBoxLayout *errLayout = new QVBoxLayout(gridWidget);
            errLayout->addWidget(errLabel);
            return;
        }

        QTextStream in(&file);
        Contributor current;
        bool hasCurrent = false;
        QStringList distinctRanges;

        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.isEmpty()) continue;

            if (line.startsWith("## ") && !line.startsWith("### ")) {
                if (hasCurrent) allContributors.append(current);
                current = Contributor();
                current.name = line.mid(3).trimmed();
                hasCurrent = true;
            }
            else if (line.startsWith("### ")) {
                if (hasCurrent) current.slogan = line.mid(4).trimmed();
            }
            else if (line.contains(":")) {
                if (!hasCurrent) continue;
                int colonIdx = line.indexOf(":");
                QString key = line.left(colonIdx).trimmed();
                QString val = line.mid(colonIdx + 1).trimmed();

                if (key == "Fecha") current.fecha = val;
                else if (key == "Rango") {
                    current.rango = val;
                    if (!distinctRanges.contains(val) && !val.isEmpty()) distinctRanges.append(val);
                }
                else if (key == "Descripción") current.descripcion = val;
                else if (key == "Correo") current.correo = val;
                else if (key == "Número") current.numero = val;
                else if (key == "Proyecto") current.proyecto = val;
                else if (key == "Trabajo") current.trabajo = val;
                else if (key == "Razón") current.razon = val;
            }
        }
        if (hasCurrent) allContributors.append(current);
        file.close();

        cmbRange->addItem("Todos");
        distinctRanges.sort();
        cmbRange->addItems(distinctRanges);

        applyFilters();
    }

    void applyFilters() {
        qDeleteAll(activeCards);
        activeCards.clear();

        if (allContributors.isEmpty()) return;

        QString query = txtSearch->text().toLower();
        QString selectedRange = cmbRange->currentText();
        int sortType = cmbSort->currentIndex();

        struct FilteredItem {
            Contributor info;
        };
        QVector<FilteredItem> filtered;

        for (const Contributor &info : allContributors) {
            bool matchesSearch = info.name.toLower().contains(query);
            bool matchesRange = (selectedRange == "Todos" || info.rango == selectedRange);

            if (matchesSearch && matchesRange) {
                filtered.append({info});
            }
        }

        if (sortType == 0) {
            std::sort(filtered.begin(), filtered.end(), [](const FilteredItem &a, const FilteredItem &b) {
                int nA = a.info.numero.toInt();
                int nB = b.info.numero.toInt();
                if (nA == 0 && a.info.numero != "0") nA = 9999;
                if (nB == 0 && b.info.numero != "0") nB = 9999;
                return nA < nB;
            });
        } else if (sortType == 1) {
            std::sort(filtered.begin(), filtered.end(), [](const FilteredItem &a, const FilteredItem &b) {
                return a.info.name.toLower() < b.info.name.toLower();
            });
        } else if (sortType == 2) {
            std::sort(filtered.begin(), filtered.end(), [](const FilteredItem &a, const FilteredItem &b) {
                return a.info.proyecto.toLower() < b.info.proyecto.toLower();
            });
        }

        QVector<ContributorCard*> visibleCards;
        for (const auto &item : filtered) {
            ContributorCard *card = new ContributorCard(item.info, isDarkMode, gridWidget);
            activeCards.append(card);
            visibleCards.append(card);
        }
        gridWidget->setCards(visibleCards);
    }
};

// =====================================================================
// ENTRADA DEL PROGRAMA
// =====================================================================
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    LyndsCorpApp window;
    window.show();
    return app.exec();
}
