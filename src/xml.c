/*
 * Copyright 2017 G. David Butler. All rights reserved.
 * License: https://github.com/gdavidbutler/xmlTrivialCallbackParser/blob/master/LICENSE
 */

#include <stdio.h>
#include <stdlib.h>
#include "xml.h"

int
xmlParse(
  xmlCb_t cb
 ,const char *s
,void *v
){
  const char *b;
  unsigned int tgM; /* maximum level for allocation */
  unsigned int tgL; /* current level */
  unsigned int tgD; /* gone deeper? for body */
  xmlSt_t *tg;
  xmlSt_t nm;
  xmlSt_t vl;
  char ers[32];

  if (!(b = s))
    return -1;
  tgD = tgL = tgM = 0;
  tg = 0;

tgEnd:
  vl.s = s;
  goto bgn;

err:
  vl.l = 1;
  vl.s = s - 1;
  nm.l = snprintf(ers, sizeof(ers), "Error@%zd", vl.s - b);
  nm.s = ers;
  if (cb)
    cb(xmlTp_Er, tgL, tg, &nm, &vl, v);
  goto rtn;

atrEq:
  for (;;) switch (*s++) {
  case '\0':
    goto rtn;

  case '\t': case '\n': case '\r': case ' ':
    break;

            case '"':
    goto atrValDq;

                                                              case '\'':
    goto atrValSq;

  default:
    goto err;
  }

nlTg:
  vl.l = 0;
  if (cb)
    cb(xmlTp_Ee, tgL, tg, 0, &vl, v);
  if (tgL)
    tgL--;
  for (;;) switch (*s++) {
  case '\0':
    goto rtn;

                                          case '>':
    goto tgEnd;

  default:
    goto err;
  }

nlAtrVal:
  vl.l = 0;
  if (cb)
    cb(xmlTp_Ea, tgL, tg, &nm, &vl, v);
  nm.l = 0;
  s--;
  for (;;) switch (*s++) {
  case '\0':
    goto rtn;

  case '\t': case '\n': case '\r': case ' ':
    break;

            case '"':
    goto atrValDq;

                                                              case '\'':
    goto atrValSq;

                                                                        case '/':
                                                    case '?':
    goto nlTg;

                                          case '>':
    goto tgEnd;

  case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J':
  case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':
  case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                      case '_':
  case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j':
  case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':
  case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
    goto atr;

  default:
    goto err;
  }

atrNm:
  nm.l = s - nm.s - 1;
  s--;
  for (;;) switch (*s++) {
  case '\0':
    goto rtn;

  case '\t': case '\n': case '\r': case ' ':
    break;

            case '"':                                         case '\'':
                                                                                  case '/':
    goto nlAtrVal;

                                case '=':
    goto atrEq;

                                          case '>':
  case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J':
  case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':
  case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                      case '_':
  case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j':
  case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':
  case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
    goto nlAtrVal;

  default:
    goto err;
  }

atr:
  nm.s = s - 1;
  for (;;) switch (*s++) {
  case '\0':
    goto rtn;

                                                    case '-': case '.':
  case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
  case ':':
  case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J':
  case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':
  case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                      case '_':
  case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j':
  case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':
  case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
    break;

  default:
    goto atrNm;
  }

atrVal:
  vl.l = s - vl.s - 1;
  if (cb) {
    if (nm.l)
      cb(xmlTp_Ea, tgL, tg, &nm, &vl, v);
    else
      cb(xmlTp_Ea, tgL, tg, &vl, &nm, v);
  }
  for (;;) switch (*s++) {
  case '\0':
    goto rtn;

  case '\t': case '\n': case '\r': case ' ':
    break;

            case '"':
    goto atrValDq;

                                                              case '\'':
    goto atrValSq;

                                                                        case '/':
    goto nlTg;

                                          case '>':
    goto tgEnd;

                                                    case '?':
    goto nlTg;

  case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J':
  case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':
  case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                      case '_':
  case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j':
  case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':
  case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
    goto atr;

  default:
    goto err;
  }

atrValDq:
  vl.s = s;
  for (;;) switch (*s++) {
  case '\0':
    goto rtn;

  case '\t': case '\n': case '\r': case ' ':
  case '!':
    break;

            case '"':
    goto atrVal;

                      case '#': case '$': case '%': case '&': case '\'':
  case '(': case ')': case '*': case '+': case ',': case '-': case '.': case '/':
  case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
  case ':': case ';': case '<': case '=': case '>': case '?': case '@':
  case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J':
  case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':
  case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
  case '[': case '\\':
  case ']': case '^': case '_': case '`':
  case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j':
  case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':
  case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
  case '{': case '|': case '}': case '~':
    break;

  default:
    goto err;
    break;
  }

atrValSq:
  vl.s = s;
  for (;;) switch (*s++) {
  case '\0':
    goto rtn;

  case '\t': case '\n': case '\r': case ' ':
  case '!': case '"': case '#': case '$': case '%': case '&':
    break;
                                                              case '\'':
    goto atrVal;

  case '(': case ')': case '*': case '+': case ',': case '-': case '.': case '/':
  case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
  case ':': case ';': case '<': case '=': case '>': case '?': case '@':
  case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J':
  case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':
  case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
  case '[': case '\\':
  case ']': case '^': case '_': case '`':
  case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j':
  case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':
  case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
  case '{': case '|': case '}': case '~':
    break;

  default:
    goto err;
  }

eTgNm:
  if (tgL)
    (tg + tgL - 1)->l = s - (tg + tgL - 1)->s - 1;
  else {
    (tg + tgL)->l = s - (tg + tgL)->s - 1;
    tgL++;
    vl.l = 0;
  }
  if (tgL <= tgD)
    vl.l = 0;
  if (cb)
    cb(xmlTp_Ee, tgL, tg, 0, &vl, v);
  if (tgL)
    tgL--;
  s--;
  for (;;) switch (*s++) {
  case '\0':
    goto rtn;

                                          case '>':
    goto tgEnd;

  default:
    goto err;
  }

eNm:
  if (tgL)
    (tg + tgL - 1)->s = s - 1;
  else {
    if (tgL == tgM) {
      void *t;

      if (!(t = realloc(tg, (tgM + 1) * sizeof(*tg))))
        goto rtn;
      tg = t;
      tgM++;
    }
    (tg + tgL)->s = s - 1;
  }
  for (;;) switch (*s++) {
  case '\0':
    goto rtn;

                                                    case '-': case '.':
  case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
  case ':':
    break;

                                          case '>':
    goto eTgNm;

  case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J':
  case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':
  case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                      case '_':
  case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j':
  case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':
  case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
    break;

  default:
    goto err;
  }

eTg:
  for (;;) switch (*s++) {
  case '\0':
    goto rtn;

  case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J':
  case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':
  case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                      case '_':
  case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j':
  case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':
  case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
    goto eNm;

  default:
    goto err;
  }

sTgNm:
  (tg + tgL)->l = s - (tg + tgL)->s - 1;
  tgD = tgL++;
  if (cb)
    cb(xmlTp_Eb, tgL, tg, 0, 0, v);
  s--;
  for (;;) switch (*s++) {
  case '\0':
    goto rtn;

  case '\t': case '\n': case '\r': case ' ':
    break;

            case '"':
    goto atrValDq;

                                                              case '\'':
    goto atrValSq;

                                                                        case '/':
    goto nlTg;

                                          case '>':
    goto tgEnd;

                                                    case '?':
    goto nlTg;

  case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J':
  case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':
  case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                      case '_':
  case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j':
  case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':
  case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
    goto atr;

  default:
    goto err;
  }

sNm:
  if (tgL == tgM) {
    void *t;

    if (!(t = realloc(tg, (tgM + 1) * sizeof(*tg))))
      goto rtn;
    tg = t;
    tgM++;
  }
  (tg + tgL)->s = s - 1;
  for (;;) switch (*s++) {
  case '\0':
    goto rtn;

                                                    case '-': case '.':
  case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
  case ':':
  case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J':
  case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':
  case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                      case '_':
  case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j':
  case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':
  case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
    break;

  default:
    goto sTgNm;
  }

sTg:
  vl.l = s - vl.s - 1;
  for (;;) switch (*s++) {
  case '\0':
    goto rtn;

  case '!':
    if (*(s + 0) == '-'
     && *(s + 1) == '-'
     && (*(s + 2) == ' ' || *(s + 2) == '\n' || *(s + 2) == '\r')) {
      for (s += 2; *s; s++)
        if ((*(s + 0) == ' ' || *(s + 0) == '\n' || *(s + 0) == '\r')
         && *(s + 1) == '-'
         && *(s + 2) == '-'
         && *(s + 3) == '>') {
          s += 4;
          goto tgEnd;
        }
      s++;
      goto rtn;
    }
    goto sNm;

                                                                        case '/':
    goto eTg;

                                                    case '?':
  case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J':
  case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':
  case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                      case '_':
  case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j':
  case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':
  case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
    goto sNm;

  default:
    goto err;
  }

bgn:
  for (;;) switch (*s++) {
  case '\0':
    goto rtn;

  case '\t': case '\n': case '\r': case ' ':
  case '!': case '"': case '#': case '$': case '%': case '&': case '\'':
  case '(': case ')': case '*': case '+': case ',': case '-': case '.': case '/':
  case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
  case ':': case ';':
    break;

                      case '<':
    goto sTg;

                                case '=': case '>': case '?': case '@':
  case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J':
  case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':
  case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
  case '[': case '\\':
  case ']': case '^': case '_': case '`':
  case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j':
  case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':
  case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
  case '{': case '|': case '}': case '~':
    break;

  default:
    goto err;
  }

rtn:
  free(tg);
  return s - b - 1;
}

int
xmlDecode(
  char *d
 ,const char *s
 ,unsigned int l
){
  const char *b;

  if (!d || !(b = s))
    return -1;
  for (; l--;) switch (*s) {
  case '&':
    if (!(s++,l--)) goto rtn; else switch (*s) {
    case 'a':
      if (!(s++,l--)) goto rtn; else switch (*s) {
      case 'm':
        if (!(s++,l--)) goto rtn; else switch (*s) {
        case 'p':
          if (!(s++,l--)) goto rtn; else switch (*s) {
          case ';':
            s++;
            *d++ = '&';
            continue;
          default:
            goto rtn;
          }
        default:
          goto rtn;
        }
      case 'p':
        if (!(s++,l--)) goto rtn; else switch (*s) {
        case 'o':
          if (!(s++,l--)) goto rtn; else switch (*s) {
          case 's':
            if (!(s++,l--)) goto rtn; else switch (*s) {
            case ';':
              s++;
              *d++ = '\'';
              continue;
            default:
              goto rtn;
            }
          default:
            goto rtn;
          }
        default:
          goto rtn;
        }
      default:
        goto rtn;
      }
    case 'g':
      if (!(s++,l--)) goto rtn; else switch (*s) {
      case 't':
        if (!(s++,l--)) goto rtn; else switch (*s) {
        case ';':
          s++;
          *d++ = '>';
          continue;
        default:
          goto rtn;
        }
      default:
        goto rtn;
      }
    case 'l':
      if (!(s++,l--)) goto rtn; else switch (*s) {
      case 't':
        if (!(s++,l--)) goto rtn; else switch (*s) {
        case ';':
          s++;
          *d++ = '<';
          continue;
        default:
          goto rtn;
        }
      default:
        goto rtn;
      }
    case 'q':
      if (!(s++,l--)) goto rtn; else switch (*s) {
      case 'u':
        if (!(s++,l--)) goto rtn; else switch (*s) {
        case 'o':
          if (!(s++,l--)) goto rtn; else switch (*s) {
          case 't':
            if (!(s++,l--)) goto rtn; else switch (*s) {
            case ';':
              s++;
              *d++ = '"';
              continue;
            default:
              goto rtn;
            }
          default:
            goto rtn;
          }
        default:
          goto rtn;
        }
      default:
        goto rtn;
      }
    case '#':
      if (!(s++,l--)) goto rtn; else switch (*s) {
        int c;

      case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
        c = *s - '0';
        for (s++; l; s++, l--) switch (*s) {
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
          c *= 10;
          c += *s - '0';
          break;
        case ';':
          goto endD;
        default:
          goto rtn;
        }
endD:
        if (!l--)
          goto rtn;
        s++;
        *d++ = c;
        continue;
      case 'x':
        if (!(s++,l--)) goto rtn; else switch (*s) {
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
          c = *s - '0';
          goto bgnH;
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
          c = 10 + (*s - 'A');
          goto bgnH;
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
          c = 10 + (*s - 'a');
bgnH:
          for (s++; l; s++, l--) switch (*s) {
          case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
            c *= 16;
            c += *s - '0';
            break;
          case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
            c *= 16;
            c += 10 + (*s - 'A');
            break;
          case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
            c *= 16;
            c += 10 + (*s - 'a');
            break;
          case ';':
            goto endH;
          default:
            goto rtn;
          }
endH:
          if (!l--)
            goto rtn;
          s++;
          *d++ = c;
          continue;
        default:
          goto rtn;
        }
      default:
        goto rtn;
      }
    default:
      goto rtn;
    }
  default:
    *d++ = *s++;
    break;
  }

rtn:
  *d = '\0';
  return s - b;
}
