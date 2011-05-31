#ifndef SETTINGS_H
#define SETTINGS_H
#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;
class QString;
class QCheckBox;
class QSpinBox;

class SettingsDialog : public QDialog
{

  Q_OBJECT

  public:
  SettingsDialog(QWidget *parent = 0);

  signals:
  void applyed(QString h, QString p, QString u);
  void canceled();

  private:
  QLabel *hostLabel;
  QLabel *portLabel;
  QLabel *urlLabel;
  QLabel *intervalLabel;
  QLabel *autoRefreshLable;
  QLineEdit *hostLineEdit;
  QLineEdit *portLineEdit;
  QLineEdit *urlLineEdit;
  QSpinBox *intervalSpinBox;
  QCheckBox *autoRefreshCheckBox;
  QPushButton *okButton;
  QPushButton *cancelButton;
  QString host;
  QString port;
  QString url;
  int interval;
  int autoRefresh;

  private slots:
  void okButtonClicked();
  void cancelButtonClicked();
  void hostChanged(QString h);
  void portChanged(QString p);
  void urlChanged(QString u);
  void intervalChanged(int i);
  void autoRefreshChanged(int a);

};

#endif
