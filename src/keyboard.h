#ifndef __H_KEYBOARD__
#define __H_KEYBOARD__

#define KEY_GRP_ESC        (0)
#define KEY_GRP_YEN        (1)
#define KEY_GRP_BS         (1)
#define KEY_GRP_TAB        (2)
#define KEY_GRP_Q          (2)
#define KEY_GRP_E          (2)
#define KEY_GRP_R          (2)
#define KEY_GRP_P          (3)
#define KEY_GRP_ATMARK     (3)
#define KEY_GRP_SQBR_LEFT  (3)
#define KEY_GRP_CR         (3)
#define KEY_GRP_A          (3)
#define KEY_GRP_S          (3)
#define KEY_GRP_D          (4)
#define KEY_GRP_F          (4)
#define KEY_GRP_H          (4)
#define KEY_GRP_SEMICOLON  (4)
#define KEY_GRP_COLON      (5)
#define KEY_GRP_SQBR_RIGHT (5)
#define KEY_GRP_X          (5)
#define KEY_GRP_C          (5)
#define KEY_GRP_V          (5)
#define KEY_GRP_M          (6)
#define KEY_GRP_COMMA      (6)
#define KEY_GRP_PERIOD     (6)
#define KEY_GRP_SLASH      (6)
#define KEY_GRP_SPACE      (6)
#define KEY_GRP_HOME       (6)
#define KEY_GRP_DEL        (6)
#define KEY_GRP_ROLLUP     (7)
#define KEY_GRP_ROLLDOWN   (7)
#define KEY_GRP_UNDO       (7)
#define KEY_GRP_LEFT       (7)
#define KEY_GRP_UP         (7)
#define KEY_GRP_RIGHT      (7)
#define KEY_GRP_DOWN       (7)
#define KEY_GRP_CLR        (7)
#define KEY_GRP_ENTER      (9)

#define KEY_BIT_ESC        (1)
#define KEY_BIT_YEN        (6)
#define KEY_BIT_BS         (7)
#define KEY_BIT_TAB        (0)
#define KEY_BIT_Q          (1)
#define KEY_BIT_E          (3)
#define KEY_BIT_R          (4)
#define KEY_BIT_P          (2)
#define KEY_BIT_ATMARK     (3)
#define KEY_BIT_SQBR_LEFT  (4)
#define KEY_BIT_CR         (5)
#define KEY_BIT_A          (6)
#define KEY_BIT_S          (7)
#define KEY_BIT_D          (0)
#define KEY_BIT_F          (1)
#define KEY_BIT_H          (3)
#define KEY_BIT_SEMICOLON  (7)
#define KEY_BIT_COLON      (0)
#define KEY_BIT_SQBR_RIGHT (1)
#define KEY_BIT_X          (3)
#define KEY_BIT_C          (4)
#define KEY_BIT_V          (5)
#define KEY_BIT_M          (0)
#define KEY_BIT_COMMA      (1)
#define KEY_BIT_PERIOD     (2)
#define KEY_BIT_SLASH      (3)
#define KEY_BIT_SPACE      (5)
#define KEY_BIT_HOME       (6)
#define KEY_BIT_DEL        (7)
#define KEY_BIT_ROLLUP     (0)
#define KEY_BIT_ROLLDOWN   (1)
#define KEY_BIT_UNDO       (2)
#define KEY_BIT_LEFT       (3)
#define KEY_BIT_UP         (4)
#define KEY_BIT_RIGHT      (5)
#define KEY_BIT_DOWN       (6)
#define KEY_BIT_CLR        (7)
#define KEY_BIT_ENTER      (6)

#define KEY_SCAN_CODE_ESC        ( KEY_GRP_ESC        * 8 + KEY_BIT_ESC        )
#define KEY_SCAN_CODE_YEN        ( KEY_GRP_YEN        * 8 + KEY_BIT_YEN        )
#define KEY_SCAN_CODE_BS         ( KEY_GRP_BS         * 8 + KEY_BIT_BS         )
#define KEY_SCAN_CODE_TAB        ( KEY_GRP_TAB        * 8 + KEY_BIT_TAB        )
#define KEY_SCAN_CODE_Q          ( KEY_GRP_Q          * 8 + KEY_BIT_Q          )
#define KEY_SCAN_CODE_E          ( KEY_GRP_E          * 8 + KEY_BIT_E          )
#define KEY_SCAN_CODE_R          ( KEY_GRP_R          * 8 + KEY_BIT_R          )
#define KEY_SCAN_CODE_P          ( KEY_GRP_P          * 8 + KEY_BIT_P          )
#define KEY_SCAN_CODE_ATMARK     ( KEY_GRP_ATMARK     * 8 + KEY_BIT_ATMARK     )
#define KEY_SCAN_CODE_SQBR_LEFT  ( KEY_GRP_SQBR_LEFT  * 8 + KEY_BIT_SQBR_LEFT  )
#define KEY_SCAN_CODE_CR         ( KEY_GRP_CR         * 8 + KEY_BIT_CR         )
#define KEY_SCAN_CODE_A          ( KEY_GRP_A          * 8 + KEY_BIT_A          )
#define KEY_SCAN_CODE_S          ( KEY_GRP_S          * 8 + KEY_BIT_S          )
#define KEY_SCAN_CODE_D          ( KEY_GRP_D          * 8 + KEY_BIT_D          )
#define KEY_SCAN_CODE_F          ( KEY_GRP_F          * 8 + KEY_BIT_F          )
#define KEY_SCAN_CODE_H          ( KEY_GRP_H          * 8 + KEY_BIT_H          )
#define KEY_SCAN_CODE_SEMICOLON  ( KEY_GRP_SEMICOLON  * 8 + KEY_BIT_SEMICOLON  )
#define KEY_SCAN_CODE_COLON      ( KEY_GRP_COLON      * 8 + KEY_BIT_COLON      )
#define KEY_SCAN_CODE_SQBR_RIGHT ( KEY_GRP_SQBR_RIGHT * 8 + KEY_BIT_SQBR_RIGHT )
#define KEY_SCAN_CODE_X          ( KEY_GRP_X          * 8 + KEY_BIT_E          )
#define KEY_SCAN_CODE_C          ( KEY_GRP_C          * 8 + KEY_BIT_C          )
#define KEY_SCAN_CODE_V          ( KEY_GRP_V          * 8 + KEY_BIT_V          )
#define KEY_SCAN_CODE_M          ( KEY_GRP_M          * 8 + KEY_BIT_M          )
#define KEY_SCAN_CODE_COMMA      ( KEY_GRP_COMMA      * 8 + KEY_BIT_COMMA      )
#define KEY_SCAN_CODE_PERIOD     ( KEY_GRP_PERIOD     * 8 + KEY_BIT_PERIOD     )
#define KEY_SCAN_CODE_SLASH      ( KEY_GRP_SLASH      * 8 + KEY_BIT_SLASH      )
#define KEY_SCAN_CODE_SPACE      ( KEY_GRP_SPACE      * 8 + KEY_BIT_SPACE      )
#define KEY_SCAN_CODE_HOME       ( KEY_GRP_HOME       * 8 + KEY_BIT_HOME       )
#define KEY_SCAN_CODE_DEL        ( KEY_GRP_DEL        * 8 + KEY_BIT_DEL        )
#define KEY_SCAN_CODE_ROLLUP     ( KEY_GRP_ROLLUP     * 8 + KEY_BIT_ROLLUP     )
#define KEY_SCAN_CODE_ROLLDOWN   ( KEY_GRP_ROLLDOWN   * 8 + KEY_BIT_ROLLDOWN   )
#define KEY_SCAN_CODE_UNDO       ( KEY_GRP_UNDO       * 8 + KEY_BIT_UNDO       )
#define KEY_SCAN_CODE_LEFT       ( KEY_GRP_LEFT       * 8 + KEY_BIT_LEFT       )
#define KEY_SCAN_CODE_UP         ( KEY_GRP_UP         * 8 + KEY_BIT_UP         )
#define KEY_SCAN_CODE_RIGHT      ( KEY_GRP_RIGHT      * 8 + KEY_BIT_RIGHT      )
#define KEY_SCAN_CODE_DOWN       ( KEY_GRP_DOWN       * 8 + KEY_BIT_DOWN       )
#define KEY_SCAN_CODE_CLR        ( KEY_GRP_CLR        * 8 + KEY_BIT_CLR        )
#define KEY_SCAN_CODE_ENTER      ( KEY_GRP_ENTER      * 8 + KEY_BIT_ENTER      )

#define KEY_SNS_ESC        (1 << KEY_BIT_ESC        )
#define KEY_SNS_YEN        (1 << KEY_BIT_YEN        )
#define KEY_SNS_BS         (1 << KEY_BIT_BS         )
#define KEY_SNS_TAB        (1 << KEY_BIT_TAB        )
#define KEY_SNS_Q          (1 << KEY_BIT_Q          )
#define KEY_SNS_E          (1 << KEY_BIT_E          )
#define KEY_SNS_R          (1 << KEY_BIT_R          )
#define KEY_SNS_P          (1 << KEY_BIT_P          )
#define KEY_SNS_ATMARK     (1 << KEY_BIT_ATMARK     )
#define KEY_SNS_SQBR_LEFT  (1 << KEY_BIT_SQBR_LEFT  )
#define KEY_SNS_CR         (1 << KEY_BIT_CR         )
#define KEY_SNS_A          (1 << KEY_BIT_A          )
#define KEY_SNS_S          (1 << KEY_BIT_S          )
#define KEY_SNS_D          (1 << KEY_BIT_D          )
#define KEY_SNS_F          (1 << KEY_BIT_F          )
#define KEY_SNS_H          (1 << KEY_BIT_H          )
#define KEY_SNS_SEMICOLON  (1 << KEY_BIT_SEMICOLON  )
#define KEY_SNS_COLON      (1 << KEY_BIT_COLON      )
#define KEY_SNS_SQBR_RIGHT (1 << KEY_BIT_SQBR_RIGHT )
#define KEY_SNS_X          (1 << KEY_BIT_X          )
#define KEY_SNS_C          (1 << KEY_BIT_C          )
#define KEY_SNS_V          (1 << KEY_BIT_V          )
#define KEY_SNS_M          (1 << KEY_BIT_M          )
#define KEY_SNS_COMMA      (1 << KEY_BIT_COMMA      )
#define KEY_SNS_PERIOD     (1 << KEY_BIT_PERIOD     )
#define KEY_SNS_SLASH      (1 << KEY_BIT_SLASH      )
#define KEY_SNS_SPACE      (1 << KEY_BIT_SPACE      )
#define KEY_SNS_HOME       (1 << KEY_BIT_HOME       )
#define KEY_SNS_DEL        (1 << KEY_BIT_DEL        )
#define KEY_SNS_ROLLUP     (1 << KEY_BIT_ROLLUP     )
#define KEY_SNS_ROLLDOWN   (1 << KEY_BIT_ROLLDOWN   )
#define KEY_SNS_UNDO       (1 << KEY_BIT_UNDO       )
#define KEY_SNS_LEFT       (1 << KEY_BIT_LEFT       )
#define KEY_SNS_UP         (1 << KEY_BIT_UP         )
#define KEY_SNS_RIGHT      (1 << KEY_BIT_RIGHT      )
#define KEY_SNS_DOWN       (1 << KEY_BIT_DOWN       )
#define KEY_SNS_CLR        (1 << KEY_BIT_CLR        )
#define KEY_SNS_ENTER      (1 << KEY_BIT_ENTER      )

#endif