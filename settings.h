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

  QString getHost();
  QString getPort();
  QString getUrl();
  int     getInterval();
  bool    getAutoRefresh();

  void setHost(QString p);
  void setPort(QString h);
  void setUrl(QString u);
  void setInterval(int i);
  void setAutoRefresh(bool a);

  signals:
  void applyed();
  void canceled();

  private:
  QLabel *hostLabel;
  QLabel *portLabel;
  QLabel *urlLabel;
  QLabel *intervalLabel;
  QLabel *autoRefreshLabel;
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
  Qt::CheckState autoRefresh;

  private slots:
  void okButtonClicked();
  void cancelButtonClicked();
  void hostChanged(QString h);
  void portChanged(QString p);
  void urlChanged(QString u);
  void intervalChanged(int i);
  void autoRefreshChanged(int a);

  protected:
  void showEvent(QShowEvent *event);

};

#endif
