/* Implementation of various validation functions for use in dbus-python.
 *
 * Copyright (C) 2006 Collabora Ltd. <http://www.collabora.co.uk/>
 *
 * Licensed under the Academic Free License version 2.1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "dbus_bindings-internal.h"

dbus_bool_t
dbus_py_validate_bus_name(const char *name,
                          dbus_bool_t may_be_unique,
                          dbus_bool_t may_be_not_unique)
{
    dbus_bool_t dot = FALSE;
    dbus_bool_t unique;
    char last;
    const char *ptr;

    if (name[0] == '\0') {
        PyErr_SetString(PyExc_ValueError, "Invalid bus name: "
                        "may not be empty");
        return FALSE;
    }
    unique = (name[0] == ':');
    if (unique && !may_be_unique) {
        PyErr_Format(PyExc_ValueError, "Invalid well-known bus name '%s':"
                     "only unique names may start with ':'", name);
        return FALSE;
    }
    if (!unique && !may_be_not_unique) {
        PyErr_Format(PyExc_ValueError, "Invalid unique bus name '%s': "
                     "unique names must start with ':'", name);
        return FALSE;
    }
    if (strlen(name) > 255) {
        PyErr_Format(PyExc_ValueError, "Invalid bus name '%s': "
                     "too long (> 255 characters)", name);
        return FALSE;
    }
    last = '\0';
    for (ptr = name + (unique ? 1 : 0); *ptr; ptr++) {
        if (*ptr == '.') {
            dot = TRUE;
            if (last == '.') {
                PyErr_Format(PyExc_ValueError, "Invalid bus name '%s': "
                             "contains substring '..'", name);
                return FALSE;
            }
            else if (last == '\0') {
                PyErr_Format(PyExc_ValueError, "Invalid bus name '%s': "
                             "must not start with '.'", name);
                return FALSE;
            }
        }
        else if (*ptr >= '0' && *ptr <= '9') {
            if (!unique) {
                if (last == '.') {
                    PyErr_Format(PyExc_ValueError, "Invalid bus name '%s': "
                                 "a digit may not follow '.' except in a "
                                 "unique name starting with ':'", name);
                    return FALSE;
                }
                else if (last == '\0') {
                    PyErr_Format(PyExc_ValueError, "Invalid bus name '%s': "
                             "must not start with a digit", name);
                    return FALSE;
                }
            }
        }
        else if ((*ptr < 'a' || *ptr > 'z') &&
                 (*ptr < 'A' || *ptr > 'Z') && *ptr != '_' && *ptr != '-') {
            PyErr_Format(PyExc_ValueError, "Invalid bus name '%s': "
                         "contains invalid character '%c'", name, *ptr);
            return FALSE;
        }
        last = *ptr;
    }
    if (last == '.') {
        PyErr_Format(PyExc_ValueError, "Invalid bus name '%s': must "
                        "not end with '.'", name);
        return FALSE;
    }
    if (!dot) {
        PyErr_Format(PyExc_ValueError, "Invalid bus name '%s': must "
                        "contain '.'", name);
        return FALSE;
    }
    return TRUE;
}

dbus_bool_t
dbus_py_validate_member_name(const char *name)
{
    const char *ptr;

    if (name[0] == '\0') {
        PyErr_SetString(PyExc_ValueError, "Invalid member name: may not "
                        "be empty");
        return FALSE;
    }
    if (strlen(name) > 255) {
        PyErr_Format(PyExc_ValueError, "Invalid member name '%s': "
                     "too long (> 255 characters)", name);
        return FALSE;
    }
    for (ptr = name; *ptr; ptr++) {
        if (*ptr >= '0' && *ptr <= '9') {
            if (ptr == name) {
                PyErr_Format(PyExc_ValueError, "Invalid member name '%s': "
                             "must not start with a digit", name);
                return FALSE;
            }
        }
        else if ((*ptr < 'a' || *ptr > 'z') &&
                 (*ptr < 'A' || *ptr > 'Z') && *ptr != '_') {
            PyErr_Format(PyExc_ValueError, "Invalid member name '%s': "
                         "contains invalid character '%c'", name, *ptr);
            return FALSE;
        }
    }
    return TRUE;
}

dbus_bool_t
dbus_py_validate_interface_name(const char *name)
{
    dbus_bool_t dot = FALSE;
    char last;
    const char *ptr;

    if (name[0] == '\0') {
        PyErr_SetString(PyExc_ValueError, "Invalid interface or error name: "
                        "may not be empty");
        return FALSE;
    }
    if (strlen(name) > 255) {
        PyErr_Format(PyExc_ValueError, "Invalid interface or error name '%s': "
                     "too long (> 255 characters)", name);
        return FALSE;
    }
    last = '\0';
    for (ptr = name; *ptr; ptr++) {
        if (*ptr == '.') {
            dot = TRUE;
            if (last == '.') {
                PyErr_Format(PyExc_ValueError, "Invalid interface or error name '%s': "
                             "contains substring '..'", name);
                return FALSE;
            }
            else if (last == '\0') {
                PyErr_Format(PyExc_ValueError, "Invalid interface or error name '%s': "
                             "must not start with '.'", name);
                return FALSE;
            }
        }
        else if (*ptr >= '0' && *ptr <= '9') {
            if (last == '.') {
                PyErr_Format(PyExc_ValueError, "Invalid interface or error name '%s': "
                             "a digit may not follow '.'", name);
                return FALSE;
            }
            else if (last == '\0') {
                PyErr_Format(PyExc_ValueError, "Invalid interface or error name '%s': "
                             "must not start with a digit", name);
                return FALSE;
            }
        }
        else if ((*ptr < 'a' || *ptr > 'z') &&
                 (*ptr < 'A' || *ptr > 'Z') && *ptr != '_') {
            PyErr_Format(PyExc_ValueError, "Invalid interface or error name '%s': "
                         "contains invalid character '%c'", name, *ptr);
            return FALSE;
        }
        last = *ptr;
    }
    if (last == '.') {
        PyErr_Format(PyExc_ValueError, "Invalid interface or error name '%s': must "
                        "not end with '.'", name);
        return FALSE;
    }
    if (!dot) {
        PyErr_Format(PyExc_ValueError, "Invalid interface or error name '%s': must "
                        "contain '.'", name);
        return FALSE;
    }
    return TRUE;
}


dbus_bool_t
dbus_py_validate_object_path(const char *path)
{
    const char *ptr;

    if (path[0] != '/') {
        PyErr_Format(PyExc_ValueError, "Invalid object path '%s': does not "
                     "start with '/'", path);
        return FALSE;
    }
    if (path[1] == '\0') return TRUE;
    for (ptr = path + 1; *ptr; ptr++) {
        if (*ptr == '/') {
            if (ptr[-1] == '/') {
                PyErr_Format(PyExc_ValueError, "Invalid object path '%s': "
                             "contains substring '//'", path);
                return FALSE;
            }
        }
        else if ((*ptr < 'a' || *ptr > 'z') &&
                 (*ptr < 'A' || *ptr > 'Z') &&
                 (*ptr < '0' || *ptr > '9') && *ptr != '_') {
            PyErr_Format(PyExc_ValueError, "Invalid object path '%s': "
                         "contains invalid character '%c'", path, *ptr);
            return FALSE;
        }
    }
    if (ptr[-1] == '/') {
        PyErr_Format(PyExc_ValueError, "Invalid object path '%s': ends "
                     "with '/' and is not just '/'", path);
        return FALSE;
    }
    return TRUE;
}

/* vim:set ft=c cino< sw=4 sts=4 et: */