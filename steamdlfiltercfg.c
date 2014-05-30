#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <libconfig.h>




static void toggle_boolean (GtkWidget *togglebutton,
                            gpointer   context_object)
{

  config_t *cfg = g_object_get_data (context_object, "cfg");
  const char * configpath = g_object_get_data (context_object, "configpath");
  config_setting_t *enabled = g_object_get_data (context_object, "enabled");
  GtkLabel * enabledlabel = g_object_get_data (context_object, "enabledlabel");

  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(togglebutton))) {
    config_setting_set_bool (enabled, 1);
    printf("Filter Enabled\n");
    gtk_label_set_text(enabledlabel, "Filter Enabled");
    }
  else {
    config_setting_set_bool (enabled, 0);
    printf("Filter Disabled\n");
    gtk_label_set_text(enabledlabel, "Filter Disabled");
  }
  config_write_file(cfg, configpath);

}
/* gtk_label_set_text(GtkLabel *label, gchar *text); */


int
main (int   argc,
      char *argv[])
{
  GtkWidget *window;
  GtkWidget *button;
  GtkWidget *toggle;	
  GtkWidget *grid, *box;
  GtkLabel  *enabledlabel = GTK_LABEL(gtk_label_new("Filter Disabled"));

  const gchar *logginglb = "Log to ~/.steamdlfilter.log";

  const char * chome = getenv("HOME");
  char precfg[100];
  char * config = strcpy(precfg, chome);
  const char * configpath = strcat(config,"/.config/steamdlfilter/config");
  config_t cfg;
  config_setting_t *enabled;
  config_init(&cfg);

  if(!config_read_file(&cfg, configpath))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),  config_error_line(&cfg), config_error_text(&cfg));
    printf("Error reading config file, if it exists at ~/.config/steamdlfilter/config, then the syntax may be incorrect.");
    config_destroy(&cfg);
    return(EXIT_FAILURE);
  }

  enabled = config_lookup (&cfg, "filterenabled");
  if (config_setting_get_bool(enabled) == 1)
    printf("Filter Enabled\n");
  else
    printf("Filter Disabled\n");

  gtk_init (&argc, &argv);
  
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "steamdlfiltercfg");

  grid = gtk_grid_new ();
  
  /*button = gtk_button_new_with_label ("Button");*/
  /* g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL); */
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
  /* need to use hbox/vbox for checkboxes, and pack hbox/vbox into grid. */


  toggle = gtk_toggle_button_new();
  box = gtk_hbox_new(1,1);
  gtk_container_add(GTK_CONTAINER(toggle),box);
  gtk_box_pack_start (GTK_BOX (box),
		      GTK_WIDGET(enabledlabel), FALSE, FALSE, 2);
  gtk_grid_attach (GTK_GRID (grid), toggle, 0, 0, 1, 1);

  g_object_set_data (G_OBJECT(window), "cfg", &cfg); 
  g_object_set_data (G_OBJECT(window), "configpath", (gpointer) configpath);
  g_object_set_data (G_OBJECT(window), "enabled", enabled); 
  g_object_set_data (G_OBJECT(window), "enabledlabel", (gpointer) enabledlabel);
  
  g_signal_connect (toggle, "toggled", G_CALLBACK(toggle_boolean), window);
  
  if (config_setting_get_bool(enabled) == 1)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(toggle), TRUE);
  toggle = gtk_toggle_button_new_with_label(logginglb);


  gtk_container_set_border_width (GTK_CONTAINER (window), 10);
  gtk_container_add (GTK_CONTAINER (window), grid);



  /*gtk_grid_attach (GTK_GRID (grid), button, 0, 1, 1, 1);*/


  gtk_widget_show_all (window);

  gtk_main ();
  config_write_file(&cfg, configpath);
  config_destroy(&cfg);
  return 0;
}


