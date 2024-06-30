import { createApp } from 'vue'
import App from './App.vue'
import { createVfm } from 'vue-final-modal'
import { createPinia } from 'pinia'
import ToastPlugin from 'vue-toast-notification'

import './assets/styles/main.scss'
import 'vue-final-modal/style.css'
import 'vue-toast-notification/dist/theme-bootstrap.css';

const vfm = createVfm()
const pinia = createPinia()

createApp(App)
  .use(vfm)
  .use(ToastPlugin)
  .use(pinia)
  .mount('#app')
