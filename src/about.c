
/*
 * Kanatest
 *
 * Copyright (C) 2001-2004, 2006 Tomasz Maka <pasp@users.sourceforge.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "gui.h"
#include "gui_utils.h"
#include "prefs.h"
#include "i18n.h"

/*--------------------------------------------------------------------*/

void
about_window_close_cb (GtkWidget *widget, GdkEvent *event, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    gdk_window_get_root_origin ((appGUI->about_window)->window,
                                &config.about_window_x, &config.about_window_y);

    gui_url_remove_links (&appGUI->about_links_list, &appGUI->about_link_index);

    gtk_widget_destroy (appGUI->about_window);
}

/*--------------------------------------------------------------------*/

gint
about_key_press_cb (GtkWidget *widget, GdkEventKey *event, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    if (event->keyval == GDK_Escape) {
        about_window_close_cb (NULL, NULL, appGUI);
        return TRUE;
    }

    return FALSE;
}

/*--------------------------------------------------------------------*/

void
about_close_button_cb (GtkWidget *widget, gpointer user_data) {

    GUI *appGUI = (GUI *)user_data;

    about_window_close_cb (NULL, NULL, appGUI);
}

/*--------------------------------------------------------------------*/

void
about_create_window (GUI *appGUI) {

GtkWidget       *vbox1;
GtkWidget       *vbox2;
GtkWidget       *hseparator;
GtkWidget       *hbuttonbox;
GtkWidget       *close_button;
GtkWidget       *notebook;
GtkWidget       *label;
GtkWidget       *scrolled_window;
GtkWidget       *viewport;
GtkTextBuffer   *entry_buffer = NULL;
GtkWidget       *text_sheet = NULL;
GtkTextIter     iter;
gchar           buffer[BUFFER_SIZE];
gint            i;

gchar text_contributors[] = {
    "                Vadim Berezniker\n"
    "                Yann Golanski\n"
    "                Avi Bercovich\n"
    "                Tero Pelander\n"
    "                Matt Langford\n"
    "                Dan\n"
    "                Wolfram Sang\n"
    "                Robin Redeker\n"
    "                Jakob Truelsen\n"
    "                Jonas Kölker\n"
    "                Taci Taclipoka\n"
    "                Marianne Ibbotson\n"
};

gchar text_postcards_address[] = {
    "                   Piotrek\n"
    "                   ul. Ku Słońcu 90\n"
    "                   71-046 Szczecin\n"
    "                   POLAND"
};

gchar text_received_postcards[] = {
    "  * Martin from Örebro (Sweden)\n"
    "  * Mario from Bremen (Germany)\n"
    "  * Angela from Madrit (Spain)\n"
    "  * Lukas from Brno (Czech Republic)\n"
    "  * Wolfram from Kyoto (Japan)\n"
    "  * Gabriel from Marseille (France)\n"
    "  * Unknown from Okinawa Islands (Japan)\n"
    "  * David from Madrit (Spain)\n"
    "  * Raúl from Collado Villalba in Madrit (Spain)\n"
    "  * Phil from Montlingen (Switzerland)\n"
    "  * Ken from Boston in Massachusetts (USA)\n"
    "  * Jerry from Wichita in Kansas (USA)\n"
    "  * Chris from Mâcon (France)\n"
    "  * Giulia and Daniele from Genova (Italy)\n"
    "  * Csaba, Brendon and Xenia from Budapest (Hungary)\n"
    "  * Kate from Upholland (UK)\n"
};

gchar text_license[] = {
    "This program is free software; you can redistribute it and/or modify "
    "it under the terms of the GNU General Public License as published by "
    "the Free Software Foundation; either version 2 of the License, or "
    "(at your option) any later version."
    "\n\n"
    "This program is distributed in the hope that it will be useful, "
    "but WITHOUT ANY WARRANTY; without even the implied warranty of "
    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
    "GNU General Public License for more details."
    "\n\n"
    "You should have received a copy of the GNU General Public License "
    "along with this program; if not, write to the Free Software "
    "Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA."
};

gchar *translators[] = {
    "de",   "Mathias Brodala",      "info@noctus.net",
    "",     "Frank Polte",          "frederyk@gmx.de",
    "es",   "Daniel Halens",        "daniel.halens@sabayonlinux.org",
    "fr",   "Adrian Courrèges",     "a.courreges@gmail.com",
    "hu",   "Szigetvári Csaba",     "csaba.szigetvari@informedia.hu",
    "it",   "Jacopo Farina",        "jacopo.farina@email.it",
    "pl",   "Piotr Mąka",           "silloz@users.sourceforge.net",
    "pt",   "Bruno Ramos",          "ladrilho@gmail.com",
    "ru",   "Taci Taclipoka",       "taclipoka@gmail.com"
};

    appGUI->about_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_transient_for (GTK_WINDOW(appGUI->about_window),GTK_WINDOW(appGUI->main_window));
    gtk_window_set_resizable (GTK_WINDOW (appGUI->about_window), TRUE);
    gtk_window_set_modal (GTK_WINDOW(appGUI->about_window), TRUE);
    gtk_container_set_border_width (GTK_CONTAINER (appGUI->about_window), 12);
    gtk_window_set_title (GTK_WINDOW (appGUI->about_window), _("About"));
    gtk_widget_set_size_request (appGUI->about_window, 470, 550);

    g_signal_connect (G_OBJECT (appGUI->about_window), "delete_event",
                        G_CALLBACK(about_window_close_cb), appGUI);
    g_signal_connect (G_OBJECT(appGUI->about_window), "key_press_event",
                        G_CALLBACK(about_key_press_cb), appGUI);

    gtk_window_move (GTK_WINDOW (appGUI->about_window),
                                config.about_window_x, config.about_window_y);

    gtk_widget_show (appGUI->about_window);

    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox1);
    gtk_container_add (GTK_CONTAINER (appGUI->about_window), vbox1);

    notebook = gtk_notebook_new ();
    gtk_notebook_set_tab_pos (GTK_NOTEBOOK (notebook), GTK_POS_TOP);
    gtk_widget_show (notebook);
    gtk_box_pack_start (GTK_BOX (vbox1), notebook, TRUE, TRUE, 0);

    /*-----------------------------------------------------------------*/

    vbox2 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox2);

    scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_box_pack_start (GTK_BOX (vbox2), scrolled_window, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_show (scrolled_window);

    viewport = gtk_viewport_new (NULL, NULL);
    gtk_widget_show (viewport);
    gtk_container_set_border_width (GTK_CONTAINER (viewport), 4);
    gtk_container_add (GTK_CONTAINER (scrolled_window), viewport);

    entry_buffer = gtk_text_buffer_new (NULL);
    gtk_text_buffer_get_iter_at_offset (entry_buffer, &iter, 0);

    gtk_text_buffer_create_tag (entry_buffer, "bold",
								"weight", PANGO_WEIGHT_ULTRABOLD, NULL);
    gtk_text_buffer_create_tag (entry_buffer, "big",
								"size", 16 * PANGO_SCALE, NULL);
    gtk_text_buffer_create_tag (entry_buffer, "italic",
								"style", PANGO_STYLE_ITALIC, NULL);
    gtk_text_buffer_create_tag (entry_buffer, "center",
								"justification", GTK_JUSTIFY_CENTER, NULL);
    gtk_text_buffer_create_tag (entry_buffer, "fixed",
								"family", "monospace", NULL);

    gtk_text_buffer_get_iter_at_offset (entry_buffer, &iter, 0);
    appGUI->about_textview = gtk_text_view_new_with_buffer (entry_buffer);
    gtk_text_view_set_pixels_above_lines (GTK_TEXT_VIEW(appGUI->about_textview), 2);
    gtk_text_view_set_left_margin (GTK_TEXT_VIEW(appGUI->about_textview), 8);
    gtk_text_view_set_right_margin (GTK_TEXT_VIEW(appGUI->about_textview), 8);
    gtk_text_view_set_editable (GTK_TEXT_VIEW (appGUI->about_textview), FALSE);
    gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (appGUI->about_textview), FALSE);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (appGUI->about_textview), GTK_WRAP_WORD);
    gtk_widget_show (appGUI->about_textview);
    gtk_container_add (GTK_CONTAINER (viewport), appGUI->about_textview);

    gui_url_setup (&appGUI->about_links_list, &appGUI->about_link_index, appGUI->about_textview, appGUI);

    gtk_text_buffer_insert (entry_buffer, &iter, "\n", -1);

#ifndef REV
    sprintf (buffer, "Kanatest %s\n", VERSION);
#else
    sprintf (buffer, "Kanatest SVN r%d\n", REV);
#endif
    gtk_text_buffer_insert_with_tags_by_name (entry_buffer, &iter,
                        buffer, -1, "big", "center", NULL);

    gtk_text_buffer_insert (entry_buffer, &iter, "\n", -1);
    gui_url_insert_link(&appGUI->about_links_list, &appGUI->about_link_index, appGUI->about_textview,
						&iter, NULL, 0, "http://clayo.org/kanatest", TRUE, appGUI);
    gtk_text_buffer_insert (entry_buffer, &iter, "\n", -1);
    g_snprintf (buffer, BUFFER_SIZE, "\n(%s %s, %s)\n\n", _("compiled on"), __DATE__, __TIME__);
    gtk_text_buffer_insert_with_tags_by_name (entry_buffer, &iter, buffer, -1, "center", "italic", NULL);

    g_snprintf (buffer, BUFFER_SIZE, "\n%s:\n", _("Programming"));
    gtk_text_buffer_insert_with_tags_by_name (entry_buffer, &iter, buffer, -1, "bold", NULL);
    gtk_text_buffer_insert (entry_buffer, &iter, "     Tomasz Mąka <", -1);
    gui_url_insert_link (&appGUI->about_links_list, &appGUI->about_link_index, appGUI->about_textview,
						 &iter, NULL, 0, "pasp@users.sourceforge.net", TRUE, appGUI);
    gtk_text_buffer_insert (entry_buffer, &iter, ">\n", -1);

    g_snprintf (buffer, BUFFER_SIZE, "\n%s:\n", _("Graphics"));
    gtk_text_buffer_insert_with_tags_by_name (entry_buffer, &iter, buffer, -1, "bold", NULL);
    gtk_text_buffer_insert (entry_buffer, &iter, "     Maja Kocoń (", -1);
    gui_url_insert_link (&appGUI->about_links_list, &appGUI->about_link_index, appGUI->about_textview,
						 &iter, NULL, 0, "http://pinky-babble.org", TRUE, appGUI);
    gtk_text_buffer_insert (entry_buffer, &iter, ")\n", -1);
    gtk_text_buffer_insert (entry_buffer, &iter, "     Piotr Mąka <", -1);
    gui_url_insert_link (&appGUI->about_links_list, &appGUI->about_link_index, appGUI->about_textview,
						 &iter, NULL, 0, "silloz@users.sourceforge.net", TRUE, appGUI);
    gtk_text_buffer_insert (entry_buffer, &iter, ">\n", -1);

    g_snprintf (buffer, BUFFER_SIZE, "\n%s:\n", _("Translators"));
    gtk_text_buffer_insert_with_tags_by_name (entry_buffer, &iter, buffer, -1, "bold", NULL);

    for (i=0; i < (sizeof(translators) / sizeof(translators[0]) / 3); i++) {
        gtk_text_buffer_insert (entry_buffer, &iter, "     ", -1);
        if (strlen(translators[i*3+0])) {
            g_snprintf (buffer, BUFFER_SIZE, "[%s]  ", translators[i*3+0]);
            gtk_text_buffer_insert_with_tags_by_name (entry_buffer, &iter, buffer, -1, "fixed", NULL);
        } else {
            gtk_text_buffer_insert_with_tags_by_name (entry_buffer, &iter, "      ", -1, "fixed", NULL);
        }
        g_snprintf (buffer, BUFFER_SIZE, "%s <", translators[i*3+1]);
        gtk_text_buffer_insert (entry_buffer, &iter, buffer, -1);
        gui_url_insert_link (&appGUI->about_links_list, &appGUI->about_link_index, appGUI->about_textview,
                             &iter, NULL, 0, translators[i*3+2], TRUE, appGUI);
        gtk_text_buffer_insert (entry_buffer, &iter, ">\n", -1);
    }

    label = gtk_label_new (_("About"));
    gtk_notebook_append_page (GTK_NOTEBOOK (notebook), vbox2, label);

    /*-----------------------------------------------------------------*/

    vbox2 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox2);

    scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_box_pack_start (GTK_BOX (vbox2), scrolled_window, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_show (scrolled_window);

    viewport = gtk_viewport_new (NULL, NULL);
    gtk_widget_show (viewport);
    gtk_container_set_border_width (GTK_CONTAINER (viewport), 4);
    gtk_container_add (GTK_CONTAINER (scrolled_window), viewport);

    entry_buffer = gtk_text_buffer_new (NULL);
    gtk_text_buffer_get_iter_at_offset (entry_buffer, &iter, 0);
    gtk_text_buffer_create_tag (entry_buffer, "underline",
								"underline", PANGO_UNDERLINE_SINGLE, NULL);
    gtk_text_buffer_create_tag (entry_buffer, "big",
								"size", 12 * PANGO_SCALE, NULL);
    gtk_text_buffer_get_iter_at_offset (entry_buffer, &iter, 0);
    text_sheet = gtk_text_view_new_with_buffer (entry_buffer);
    gtk_text_view_set_pixels_above_lines (GTK_TEXT_VIEW(text_sheet), 2);
    gtk_text_view_set_left_margin (GTK_TEXT_VIEW(text_sheet), 8);
    gtk_text_view_set_right_margin (GTK_TEXT_VIEW(text_sheet), 8);
    gtk_text_view_set_editable (GTK_TEXT_VIEW (text_sheet), FALSE);
    gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (text_sheet), FALSE);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (text_sheet), GTK_WRAP_WORD);
    gtk_widget_show (text_sheet);
    gtk_container_add (GTK_CONTAINER (viewport), text_sheet);

    g_snprintf (buffer, BUFFER_SIZE, "%s\n\n",
				_("We would like to thank the following people for suggestions, bug reports and patches:"));
    gtk_text_buffer_insert (entry_buffer, &iter, buffer, -1);

    gtk_text_buffer_insert (entry_buffer, &iter, text_contributors, -1);

    label = gtk_label_new (_("Contributors"));
    gtk_notebook_append_page (GTK_NOTEBOOK (notebook), vbox2, label);

    /*-----------------------------------------------------------------*/

    vbox2 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox2);

    scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_box_pack_start (GTK_BOX (vbox2), scrolled_window, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_show (scrolled_window);

    viewport = gtk_viewport_new (NULL, NULL);
    gtk_widget_show (viewport);
    gtk_container_set_border_width (GTK_CONTAINER (viewport), 4);
    gtk_container_add (GTK_CONTAINER (scrolled_window), viewport);

    entry_buffer = gtk_text_buffer_new (NULL);
    gtk_text_buffer_get_iter_at_offset (entry_buffer, &iter, 0);
    gtk_text_buffer_create_tag (entry_buffer, "underline",
                  "underline", PANGO_UNDERLINE_SINGLE, NULL);
    gtk_text_buffer_create_tag (entry_buffer, "big",
                  "size", 12 * PANGO_SCALE, NULL);
    gtk_text_buffer_create_tag (entry_buffer, "center",
                  "justification", GTK_JUSTIFY_CENTER, NULL);
    gtk_text_buffer_get_iter_at_offset (entry_buffer, &iter, 0);
    text_sheet = gtk_text_view_new_with_buffer (entry_buffer);
    gtk_text_view_set_pixels_above_lines (GTK_TEXT_VIEW(text_sheet), 2);
    gtk_text_view_set_left_margin (GTK_TEXT_VIEW(text_sheet), 8);
    gtk_text_view_set_right_margin (GTK_TEXT_VIEW(text_sheet), 8);
    gtk_text_view_set_editable (GTK_TEXT_VIEW (text_sheet), FALSE);
    gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (text_sheet), FALSE);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (text_sheet), GTK_WRAP_WORD);
    gtk_widget_show (text_sheet);
    gtk_container_add (GTK_CONTAINER (viewport), text_sheet);

    g_snprintf (buffer, BUFFER_SIZE, "%s:\n\n",
                _("If you like our program, send a postcard with a photo of the city where you live to"));
    gtk_text_buffer_insert (entry_buffer, &iter, buffer, -1);

    gtk_text_buffer_insert (entry_buffer, &iter, text_postcards_address, -1);

    g_snprintf (buffer, BUFFER_SIZE, "\n\n%s:\n\n",
                _("We have received several postcards from our happy users"));
    gtk_text_buffer_insert (entry_buffer, &iter, buffer, -1);

    gtk_text_buffer_insert (entry_buffer, &iter, text_received_postcards, -1);

    g_snprintf (buffer, BUFFER_SIZE, "\n%s\n",
                _("Your postcards are a kind of motivation that encourages us to keep improving Kanatest :)"));
    gtk_text_buffer_insert (entry_buffer, &iter, buffer, -1);
    g_snprintf (buffer, BUFFER_SIZE, "\n%s\n", _("Thank you!"));
    gtk_text_buffer_insert_with_tags_by_name (entry_buffer, &iter, buffer, -1, "center", NULL);

    label = gtk_label_new (_("Postcards"));
    gtk_notebook_append_page (GTK_NOTEBOOK (notebook), vbox2, label);

    /*-----------------------------------------------------------------*/

    vbox2 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox2);

    scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_box_pack_start (GTK_BOX (vbox2), scrolled_window, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
                                    GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_show (scrolled_window);

    viewport = gtk_viewport_new (NULL, NULL);
    gtk_widget_show (viewport);
    gtk_container_set_border_width (GTK_CONTAINER (viewport), 4);
    gtk_container_add (GTK_CONTAINER (scrolled_window), viewport);

    entry_buffer = gtk_text_buffer_new (NULL);
    gtk_text_buffer_get_iter_at_offset (entry_buffer, &iter, 0);
    gtk_text_buffer_create_tag (entry_buffer, "underline",
								"underline", PANGO_UNDERLINE_SINGLE, NULL);
    gtk_text_buffer_create_tag (entry_buffer, "big",
								"size", 12 * PANGO_SCALE, NULL);
    text_sheet = gtk_text_view_new_with_buffer (entry_buffer);
    gtk_text_view_set_pixels_above_lines (GTK_TEXT_VIEW(text_sheet), 2);
    gtk_text_view_set_left_margin (GTK_TEXT_VIEW(text_sheet), 8);
    gtk_text_view_set_right_margin (GTK_TEXT_VIEW(text_sheet), 8);
    gtk_text_view_set_editable (GTK_TEXT_VIEW (text_sheet), FALSE);
    gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (text_sheet), FALSE);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (text_sheet), GTK_WRAP_WORD);
    gtk_widget_show (text_sheet);
    gtk_container_add (GTK_CONTAINER (viewport), text_sheet);

    gtk_text_buffer_insert (entry_buffer, &iter, text_license, -1);

    label = gtk_label_new (_("License"));
    gtk_notebook_append_page (GTK_NOTEBOOK (notebook), vbox2, label);

    /*-----------------------------------------------------------------*/

    hseparator = gtk_hseparator_new ();
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (vbox1), hseparator, FALSE, TRUE, 8);

    hbuttonbox = gtk_hbutton_box_new ();
    gtk_widget_show (hbuttonbox);
    gtk_button_box_set_layout (GTK_BUTTON_BOX(hbuttonbox), GTK_BUTTONBOX_END);
    gtk_box_pack_start (GTK_BOX (vbox1), hbuttonbox, FALSE, TRUE, 0);

    close_button = gtk_button_new_from_stock (GTK_STOCK_CLOSE);
    gtk_widget_show (close_button);
    g_signal_connect (G_OBJECT (close_button), "clicked",
                        G_CALLBACK (about_close_button_cb), appGUI);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), close_button);
    GTK_WIDGET_SET_FLAGS (close_button, GTK_CAN_DEFAULT);

    gtk_widget_grab_default (close_button);

}

/*--------------------------------------------------------------------*/
