#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>

#define BUFSIZE 1024
char *cmd;
char *imagename;
// char *haarImage;
// gboolean notHaar = FALSE;
gboolean compareNames(char *a, char *b)
{
  gboolean isSame = TRUE;

  size_t Asize = sizeof(a) / sizeof(char *);
  size_t Bsize = sizeof(b) / sizeof(char *);
  size_t i = 0;
  while (i < Asize && i < Bsize && isSame)
  {
    if (a[i] != b[i])
    {
      isSame = FALSE;
    }
    return isSame;
  }
}
static gint parse_output(GtkButton *button, gpointer user_data)
{

  GtkEntry *entry = user_data;
  const gchar *text = gtk_button_get_label(button);
 
 //LBP ******************************************************************************
  if (compareNames(text, "LBP"))
  {
    char *tmpO = "./haar.sh ";
    printf("tmp %s \n", tmpO);
    cmd = (char *)malloc(1 + strlen(tmpO) + strlen(imagename));

    strcpy(cmd, tmpO);
    strcat(cmd, imagename);
    printf("cmd %s \n", cmd);
    char buf[BUFSIZE];
    FILE *fp;

    if ((fp = popen(cmd, "r")) == NULL)
    {
      printf("Error opening pipe!\n");
      return -1;
    }

    while (fgets(buf, BUFSIZE, fp) != NULL)
    {
      printf("OUTPUT: %s", buf);
    }
    if (pclose(fp))
    {
      printf("Command not found or exited with error status\n");
      return -1;
    }

    char *tmp = "./matchLBPGUI ";
     printf("tmp %s \n", tmp);
    cmd = (char *)malloc(1024);
    // printf("cmd alloué \n");
    strcpy(cmd, tmp);
    printf("cmd copié : %s \n", cmd);
    // printf("rappel imagename : %s \n", imagename);

    strcat(cmd, "./img/haar.pgm");
  }

  
 //Haar ******************************************************************************
  if (compareNames(text, "Haar"))
  {
    char *tmp = "./haar.sh ";
    printf("tmp %s \n", tmp);
    cmd = (char *)malloc(1 + strlen(tmp) + strlen(imagename));

    strcpy(cmd, tmp);
    strcat(cmd, imagename);
    printf("cmd %s \n", cmd);
  }



 //EIGNE ******************************************************************************
  if (compareNames(text, "EigenFaces"))
  {

    char *tmpO = "./haar.sh ";
    printf("tmp %s \n", tmpO);
    cmd = (char *)malloc(1 + strlen(tmpO) + strlen(imagename));

    strcpy(cmd, tmpO);
    strcat(cmd, imagename);
    printf("cmd %s \n", cmd);

    char buf[BUFSIZE];
    FILE *fp;

    if ((fp = popen(cmd, "r")) == NULL)
    {
      printf("Error opening pipe!\n");
      return -1;
    }

    while (fgets(buf, BUFSIZE, fp) != NULL)
    {
      printf("OUTPUT: %s", buf);
    }
    if (pclose(fp))
    {
      printf("Command not found or exited with error status\n");
      return -1;
    }

    char *tmp = "./matchEigenGUI ../Code/out/eigenfaces ";
    printf("tmp %s \n", tmp);
    cmd = (char *)malloc(1024);
    // printf("cmd alloué \n");
    strcpy(cmd, tmp);
    printf("cmd copié : %s \n", cmd);
    // printf("rappel imagename : %s \n", imagename);

    strcat(cmd, "./img/haar.pgm");

    // printf("cmd %s \n", cmd);
  }
  // char *cmd;
  // cmd = (char *)data;
  g_print("Trying to compute %s \n", cmd);
  char buf[BUFSIZE];
  FILE *fp;

  if ((fp = popen(cmd, "r")) == NULL)
  {
    printf("Error opening pipe!\n");
    return -1;
  }

  while (fgets(buf, BUFSIZE, fp) != NULL)
  {
    printf("OUTPUT: %s", buf);
  }
  if (pclose(fp))
  {
    printf("Command not found or exited with error status\n");
    return -1;
  }

  return 0;
}

float HEIGHT = 800;
float WIDTH = 600;

void emptyWindow(GtkWidget *window)
{
  GList *children, *iter;

  children = gtk_container_get_children(GTK_CONTAINER(window));
  for (iter = children; iter != NULL; iter = g_list_next(iter))
  {
    // iter->g_gtype_get_type();
    //  gtk_container_remove(iter,window);
    gtk_widget_destroy(GTK_WIDGET(iter->data));
  }

  g_list_free(children);
}

static void openImage(GtkWidget *parent_window);

static gboolean button_openImage(GtkWidget *event_box,
                                 GdkEventButton *event,
                                 gpointer data)
{
  g_print("Event box clicked at coordinates %f,%f\n",
          event->x, event->y);
  GtkWidget *window = (GtkWidget *)data;
  openImage(window);
  // Returning TRUE means we handled the event, so the signal
  // emission should be stopped (don’t call any further callbacks
  // that may be connected). Return FALSE to continue invoking callbacks.
  return TRUE;
}

static void openImage(GtkWidget *button)
{
  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
  gint res;
  GtkWidget *image = NULL;
  GtkWidget *pVBox;
  GtkWidget *rButtonVBox;
  GtkWidget *parent_window;
  GtkWidget *halign, *halignRButton;
  GtkWidget *LBPbutton, *Ebutton, *CNNbutton;

  // This is used to get the parent window, i.e the one that contains the button that called this callback
  GdkWindow *gtk_window = gtk_widget_get_parent_window(button);
  parent_window = NULL;
  gdk_window_get_user_data(gtk_window, (gpointer *)&parent_window);
  g_print("%s\n", gtk_window_get_title(parent_window));

  emptyWindow(parent_window);

  // After emptying, we create a new box to store our future widgets.
  pVBox = gtk_vbox_new(FALSE, 0);
  rButtonVBox = gtk_hbox_new(TRUE, 0);
  gtk_container_add(GTK_CONTAINER(parent_window), pVBox);

  dialog = gtk_file_chooser_dialog_new("Open File",
                                       parent_window,
                                       action,
                                       ("_Cancel"),
                                       GTK_RESPONSE_CANCEL,
                                       ("_Open"),
                                       GTK_RESPONSE_ACCEPT,
                                       NULL);

  res = gtk_dialog_run(GTK_DIALOG(dialog));

  if (res == GTK_RESPONSE_ACCEPT)
  {
    char *filename;
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
    filename = gtk_file_chooser_get_filename(chooser);
    imagename = (char *)malloc(1 + strlen(filename));
    strcpy(imagename, filename);
    g_print("filename in callback %s\n",
            filename);
    printf("imagename %s\n", imagename);
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
    pixbuf = gdk_pixbuf_scale_simple(pixbuf, HEIGHT, WIDTH, GDK_INTERP_BILINEAR);
    image = gtk_image_new_from_pixbuf(pixbuf);

    g_free(filename);
  }

  // redrawing new button
  button = gtk_button_new_with_label("Open another image");
  gtk_widget_set_tooltip_text(button, "Select another image for recognition");
  g_signal_connect(G_OBJECT(button), "clicked",
                   G_CALLBACK(button_openImage), (gpointer)&parent_window);
  halign = gtk_alignment_new(0, 0, 0, 0);
  gtk_container_add(GTK_CONTAINER(halign), button);
  halignRButton = gtk_alignment_new(0.5, 0, 0, 0);

  LBPbutton = gtk_button_new_with_label("LBP");
  gtk_widget_set_tooltip_text(LBPbutton, "Launch a recognition using LBP");
  g_signal_connect(G_OBJECT(LBPbutton), "clicked",
                   G_CALLBACK(parse_output), NULL);

  gtk_container_add(GTK_CONTAINER(rButtonVBox), LBPbutton);

  Ebutton = gtk_button_new_with_label("EigenFaces");
  gtk_widget_set_tooltip_text(Ebutton, "Launch a recognition using EigenFaces");
  g_signal_connect(G_OBJECT(Ebutton), "clicked",
                   G_CALLBACK(parse_output), NULL);
  gtk_container_add(GTK_CONTAINER(rButtonVBox), Ebutton);

  CNNbutton = gtk_button_new_with_label("Haar");
  gtk_widget_set_tooltip_text(CNNbutton, "Compute Haar on the selected image");
  g_signal_connect(G_OBJECT(CNNbutton), "clicked",
                   G_CALLBACK(parse_output), NULL);
  gtk_container_add(GTK_CONTAINER(rButtonVBox), CNNbutton);

  gtk_container_add(GTK_CONTAINER(halignRButton), rButtonVBox);
  // Dialog box to choose the image
  gtk_box_pack_start(GTK_BOX(pVBox), halign, FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(pVBox), image, FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(pVBox), halignRButton, FALSE, FALSE, 0);
  gtk_widget_show_all(parent_window);
  gtk_widget_destroy(dialog);
}

// updateWindow(GtkWidget *event_box,
//              GdkEventButton *event,
//              gpointer data)
// {
//   g_print("Window update requested");

//   return TRUE;
// }
/***         ###                 ------             ###            ***/
int main(int argc, char *argv[])
{

  GtkWidget *window;
  GtkWidget *button;
  GtkWidget *halign;
  // GtkWidget *image = NULL;
  GtkWidget *pVBox;
  // char *imagename;
  // bool *needUpdate;
  // *needUpdate = false;
  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Face Recognition");
  gtk_window_set_default_size(GTK_WINDOW(window), HEIGHT, WIDTH);
  gtk_container_set_border_width(GTK_CONTAINER(window), 15);
  pVBox = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), pVBox);

  button = gtk_button_new_with_label("Open an image");
  gtk_widget_set_tooltip_text(button, "Select an image for recognition");
  g_signal_connect(G_OBJECT(button), "clicked",
                   G_CALLBACK(button_openImage), (gpointer)&window);
  // g_signal_connect(G_OBJECT(button), "clicked",
  //                  G_CALLBACK(updateWindow), (gpointer)&needUpdate);

  // if(*needUpdate){

  // }

  halign = gtk_alignment_new(0, 0, 0, 0);
  gtk_container_add(GTK_CONTAINER(halign), button);
  gtk_box_pack_start(GTK_BOX(pVBox), halign, FALSE, FALSE, 5);

  gtk_widget_show_all(window);

  g_signal_connect(G_OBJECT(window), "destroy",
                   G_CALLBACK(gtk_main_quit), NULL);
  gtk_main();

  return 0;
}